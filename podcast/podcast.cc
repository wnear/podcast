#include <QFileDialog>
#include <QDomDocument>
#include <QDebug>

#include <QFile>
#include <QStandardPaths>
#include <QMenu>

#include <QTabWidget>
#include <QListView>
#include <QVBoxLayout>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include <QListWidget>
#include <QListWidgetItem>
#include <QGuiApplication>
#include <QClipboard>

#include <QScrollArea>

#include "podcast.h"
#include "opmlparser.h"
// #include "episodelistwgt.h"
#include "episode_treewidget.h"
#include "episode_detail_wgt.h"
#include "podcastchannel.h"
#include "podmodel.h"
#include "sqlmanager.h"

using namespace std;
using namespace util;
class Podcast::Private {
  public:
    QListView *listview;
    QTabWidget *detail;
    // EpisodeListWidget *detaillist;
    EpisodeTreeWidget *detailtree;
    PodModel *podsmodel;
    QString lastxml;
    QString totalxml;
    QNetworkAccessManager *net;
};

Podcast::Podcast(QWidget *parent) : QWidget(parent) {
    d = new Private;
    load();
    d->net = new QNetworkAccessManager();

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    d->listview = new QListView(this);
    {
        // d->list->setViewMode(QListView::IconMode);
        // d->list->setResizeMode(QListView::Adjust);
        d->podsmodel = new PodModel(m_channels, this);
        d->listview->setModel(d->podsmodel);
        d->listview->setContextMenuPolicy(Qt::CustomContextMenu);
    }
    lay->addWidget(d->listview);

    d->detail = new QTabWidget(this);
    // d->detaillist = new EpisodeListWidget(this);
    d->detailtree = new EpisodeTreeWidget(this);
    d->detail->addTab(d->detailtree, "modal list");

    assert(d->listview != nullptr);

    connect(d->detailtree, &EpisodeTreeWidget::requestPlay, this, &Podcast::requestPlay);
    connect(d->detailtree, &EpisodeTreeWidget::requestDetail, this,
            &Podcast::requestDetail);

    connect(d->listview, &QWidget::customContextMenuRequested, this,
            [this](const QPoint &pos) {
                auto idx = d->listview->indexAt(pos);
                int row = idx.row();
                PodcastChannel &pod = *m_channels[row];
                pod.load();
                auto menu = new QMenu(this);
                menu->addAction("[debug]reparse", this, [&pod]() { pod.parserxml(); });
                menu->addAction("[debug]clear all episodes", this,
                                [&pod]() { pod.clearEpisodes(); });

                menu->addAction("update", this, [&pod]() { pod.updatexml(); });
                menu->addAction("copy url", this, [&pod]() {
                    QClipboard *clipboard = QGuiApplication::clipboard();
                    clipboard->setText(pod.url());
                });
                menu->exec(mapToGlobal(pos));
            });
    connect(d->listview, &QListView::clicked, [this](auto &&idx) {
        // title/ url ==> feed ==> detailview ==> detailview update.

        auto model = qobject_cast<PodModel *>(d->listview->model());
        auto url = model->data(idx, PodModel::UrlRole).toString();
        int row = idx.row();
        PodcastChannel &pod = *m_channels[row];
        pod.load();
        int cnt = m_channels[row]->m_episodes.count();
        qDebug() << "by load from cache, get episodes of count: " << cnt;
        // d->detaillist->setPod( m_pods[row]);
        d->detailtree->setPod(m_channels[row]);
    });
}

bool Podcast::save() {
    auto path = util::datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile f(podsfile);
    if (f.open(QIODevice::WriteOnly) == false) {
        qDebug() << "Err to open for write";
        return false;
    }

    QJsonArray whole;
    for (auto i : m_channels) {
        whole.push_back(QJsonObject{{"title", i->m_feedTitle}, {"url", i->m_feedUrl}});
    }
    QJsonDocument doc;
    doc.setArray(whole);
    f.write(doc.toJson());
    return true;
}

bool Podcast::load() {
    SQLManager::instance()->loadChannels(m_channels);
    qDebug() << "podcast, channels:" << m_channels.size();
    return true;

    auto path = util::datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile pods(podsfile);
    if (pods.exists() == false || pods.open(QIODevice::ReadOnly) == false) {
        qDebug() << "fail to open subscription.json file:"
                 << QFileInfo(pods).absoluteFilePath();
        return false;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(pods.readAll(), &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug() << "Json parse error: " << err.errorString();
        return false;
    }
    for (auto i : doc.array()) {
        auto item = i.toObject();

        auto x = new PodcastChannel(item.value("title").toString(),
                                    item.value("url").toString());

        m_channels.push_back(x);
    }
    return true;
}

QWidget *Podcast::detail() const { return d->detail; }


void Podcast::importdlg() {
    QString filename = QFileDialog::getOpenFileName(/*paent wgt*/ this,
                                                    /*caption*/ "get opml file", "",
                                                    "Opml(*.opml *.xml);;All(*)");
    if (filename.isEmpty()) return;
    import_opml(filename);
}

void Podcast::import_opml(const QString &filename) {
    auto channels = OpmlParser(filename).parse();
    if (channels.isEmpty()) return;

    QList<std::pair<channel_item_t, channel_item_t>> conflicts;
    while (1) {
        SQLManager::instance()->addChannels(m_channels, channels, conflicts);
        if (conflicts.empty())
            break;
        else {
            // dialog to choose from list for channel list info to add.
        }
        // TODO: implement the else dislog.
        break;
    }
    // qDebug() << QString("import result: sum(%1), added(%2), ignored(%3)")
    //                 .arg(channels.size())
    //                 .arg(added)
    //                 .arg(alreadyhave);

    d->listview->reset();

    save();
}

// update with network.
// mannually update, update upon click.
/**
1. request with pod.url.
2. wait for response.
3. parse response
4. add data => pod -> other field.
5. add episode => pod->episodes
6. sync pod data => local json
 */

// load from local storage.
void Podcast::pod_load_episodes(PodcastChannel &pod) {
    // auto ret = pod_load_with_xml(pod);
    auto ret = pod.load();
    if (ret == false) return;
}

void Podcast::exportdlg() {}
// TODO: export.
void Podcast::export_opml(const QString &filename) {}
