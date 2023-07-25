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
    QListView *list;
    QTabWidget *detail;
    // EpisodeListWidget *detaillist;
    EpisodeTreeWidget *detailtree;
    EpisodeDetailWidget *ep_detail;
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
    d->list = new QListView(this);
    {
        // d->list->setViewMode(QListView::IconMode);
        // d->list->setResizeMode(QListView::Adjust);
        d->podsmodel = new PodModel(m_channels, this);
        d->list->setModel(d->podsmodel);
        d->list->setContextMenuPolicy(Qt::CustomContextMenu);
    }
    lay->addWidget(d->list);

    d->detail = new QTabWidget(this);
    // d->detaillist = new EpisodeListWidget(this);
    d->detailtree = new EpisodeTreeWidget(this);
    d->ep_detail = new EpisodeDetailWidget(this);
    d->detail->addTab(d->detailtree, "modal list");
    d->detail->addTab(d->ep_detail, "ep detail");

    assert(d->list != nullptr);

    connect(d->detailtree, &EpisodeTreeWidget::requestPlay, this, &Podcast::requestPlay);
    connect(d->detailtree, &EpisodeTreeWidget::requestDetail, this,
            &Podcast::requestDetail);

    connect(d->list, &QWidget::customContextMenuRequested, this,
            [this](const QPoint &pos) {
                auto idx = d->list->indexAt(pos);
                int row = idx.row();
                PodcastChannel &pod = *m_channels[row];
                pod.load();
                auto menu = new QMenu(this);
                menu->addAction("reparse", this, [&pod]() { pod.parserxml(); });
                menu->addAction("update", this, [&pod]() { pod.updatexml(); });
                menu->exec(mapToGlobal(pos));
            });
    connect(d->list, &QListView::clicked, [this](auto &&idx) {
        // title/ url ==> feed ==> detailview ==> detailview update.

        auto model = qobject_cast<PodModel *>(d->list->model());
        auto url = model->data(idx, PodModel::UrlRole).toString();
        int row = idx.row();
        PodcastChannel &pod = *m_channels[row];
        pod.load();
        int cnt = m_channels[row]->episodes.count();
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
    auto path = util::datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile pods(podsfile);
    if (pods.exists() == false || pods.open(QIODevice::ReadOnly) == false) {
        qDebug() << "fail to open";
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
        // m_pods.push_back(std::move(PodcastChannel(item.value("title").toString(),
        // item.value("url").toString())));
    }
    return true;
}

QWidget *Podcast::detail() const { return d->detail; }

EpisodeDetailWidget *Podcast::ep_detail() const { return d->ep_detail; }

void Podcast::importdlg() {
    QString filename = QFileDialog::getOpenFileName(/*paent wgt*/ this,
                                                    /*caption*/ "get opml file");
    /*dir*/
    /*filter*/
    if (filename.isEmpty()) return;
    import_opml(filename);
}

void Podcast::import_opml(const QString &filename) {
    auto res = OpmlParser(filename).parse();
    if (res.isEmpty()) return;

    auto alreadyHave = [this](const QString &title, const QString &url) {
        auto i = std::find_if(m_channels.begin(), m_channels.end(),
                              [url, title](auto &&p) { return p->m_feedUrl == url; });
        return i != m_channels.end();
    };

    for (auto [title, url] : res) {
        if (!alreadyHave(title, url)){
            m_channels.push_back(new PodcastChannel(title, url));
        }
        if(SQLManager::instance()->findChannel(title, url)){
            //prompt dialog.
            assert(0);
        } else {
            SQLManager::instance()->addChannel(title, url);
        }

    }

    // d->podsmodel->resetData(m_pods);
    d->list->reset();

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
