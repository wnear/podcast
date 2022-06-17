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

#include "episodewgt.h"

#include "podcast.h"
#include "rssparser.h"
#include "episodelistwgt.h"
#include "episode_treewidget.h"

#include "downloadmanager.h"
#include "log.h"
#include "jsonop.h"
#include "global.h"

namespace {

    QString ensureDirExist(const QString &parent, const QString &dirname)
    {
        auto path = QDir(parent).filePath(dirname);
        if(QFileInfo::exists(path) == false) {
            QDir(parent).mkdir(dirname);
        } else {
        // use empty string to as error.
            if(QFileInfo(path).isDir() == false)
                return "";
        }
        return path;
    }
}

class Podcast::Private{
public:
    QListView *list;
    QTabWidget *detail;
    EpisodeListWidget *detaillist;
    EpisodeTreeWidget *detailtree;
    PodModel *podsmodel;
    QString lastxml;
    QString totalxml;
    QNetworkAccessManager *net;
};

Podcast::Podcast(QWidget *parent): QWidget(parent)
{
    d = new Private;
    load();
    d->net = new QNetworkAccessManager();
    QVBoxLayout *lay = new QVBoxLayout(this);
    d->list = new QListView(this);
    d->detail = new QTabWidget(this);
    d->detaillist = new EpisodeListWidget(this);
    d->detailtree = new EpisodeTreeWidget(this);
    d->detail->addTab(d->detailtree, "modal list");
    d->detail->addTab(d->detaillist, "diy list");
    d->podsmodel = new PodModel(m_pods, this);
    d->list->setModel(d->podsmodel);

    
    lay->addWidget(d->list);

    d->list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(d->list, &QWidget::customContextMenuRequested, this, [this](const QPoint &pos){
                auto idx = d->list->indexAt(pos);
                int row = idx.row();
                PodData &pod = *m_pods[row];
                podLoad(*m_pods[row]);
                auto menu = new QMenu(this);
                menu->addAction("reparse", this, [this, &pod](){
                                    this->parsexml(pod);
                                });
                menu->addAction("update", this, [this, &pod](){
                                    this->updatexml(pod);
                                });
                menu->exec(mapToGlobal(pos));
            });
    connect(d->list, &QListView::clicked, [this](auto &&idx){
                // title/ url ==> feed ==> detailview ==> detailview update.
                auto model = qobject_cast<PodModel *>(d->list->model());
                auto url = model->data(idx, PodModel::UrlRole).toString();
                int row = idx.row();
                podLoad(*m_pods[row]);
                int cnt = m_pods[row]->episodes.count();
                qDebug()<<"by load from cache, get episodes of count: " << cnt;
                d->detaillist->setPod( m_pods[row]);
                d->detailtree->setPod( m_pods[row]);
            });
}

bool Podcast::save()
{
    auto path = this->datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile f(podsfile);
    if(f.open(QIODevice::WriteOnly) == false){
        qDebug()<<"Err to open for write";
        return false;
    }

    QJsonArray whole;
    for(auto i: m_pods){
        whole.push_back(QJsonObject{{"title", i->title}, {"url", i->url}});
    }
    QJsonDocument doc;
    doc.setArray(whole);
    f.write(doc.toJson());
    return true;
}

bool Podcast::load()
{
    auto path = this->datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile pods(podsfile);
    if(pods.exists() == false || pods.open(QIODevice::ReadOnly) == false){
        qDebug()<<"fail to open";
        return false;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(pods.readAll(), &err);
    if(err.error != QJsonParseError::NoError) {
        qDebug()<<"Json parse error: "<<err.errorString();
        return false;
    }
    for(auto i: doc.array()) {
        auto item = i.toObject();
        
        auto x = new PodData(item.value("title").toString(),
                                 item.value("url").toString());
        m_pods.push_back(x);
        //m_pods.push_back(std::move(PodData(item.value("title").toString(),
                                 //item.value("url").toString())));
    }
    return true;
}


QWidget *Podcast::detail() const 
{ return d->detail; }


QString Podcast::datapath() const
{
    QString app_datapath = QStandardPaths::writableLocation(
                                QStandardPaths::AppLocalDataLocation);
    return ensureDirExist(app_datapath, "podcast");
}

QString Podcast::datapath(PodData &pod) const
{ return ensureDirExist(datapath(), pod.title); }

void Podcast::importdlg()
{
    QString filename = QFileDialog::getOpenFileName(/*paent wgt*/this, 
                                                /*caption*/"get opml file");
                                                /*dir*/
                                                /*filter*/
    if(filename.isEmpty())
        return;
    read_opml(filename);
}

void Podcast::read_opml(const QString &filename)
{
    QDomDocument docu;
    QFile xml(filename);
    if(xml.exists() == false || xml.open(QIODevice::ReadOnly) == false){
        qDebug()<<"Error to open opml file";
        return;
    }
    docu.setContent(xml.readAll());
    auto nodes = docu.elementsByTagName("outline");
    auto alreadyHave = [this](const PodData &now){
        auto i = std::find_if(m_pods.begin(), m_pods.end(), 
                              [now](auto && p){
                                  return p->url == now.url;
                              });
        return i != m_pods.end();
    };
    int sum = nodes.length();
    int added = 0;
    for(decltype(nodes.length()) i = 0; i < nodes.length(); i++) {
        auto node = nodes.item(i);
        if(node.isElement()){
            qDebug()<<"idx = "<<i;
            auto elnode = node.toElement();
            if(elnode.isNull())
                continue;
            auto x = new PodData(elnode.attribute("text", ""),
                      elnode.attribute("xmlUrl", ""));
            if(x->isValid()) {
                if(alreadyHave(*x)){
                }else {
                    added += 1;
                    m_pods.push_back(x);
                }
            } else {
                delete x;
            }
        }
    }
    qDebug()<< QString("result: %1 of %2 is added").arg(added).arg(sum);
    d->podsmodel->resetData(m_pods);
    d->list->reset();

    save();
}

void Podcast::savepod(QString title, QString url) {
    m_pods.push_back(new PodData{title, url});
}

bool Podcast::save(PodData &pod){
    auto file = QDir(datapath(pod)).filePath("pods_detail.json");
    return jsonsave(&pod, file);
}

bool Podcast::load(PodData &pod){
    auto isNewer = [](const QFile &lhs, const QFile &rhs){
        bool ret = QFileInfo(lhs).lastModified() > QFileInfo(rhs).lastModified();
        qDebug()<<"xml is "<<(ret?"newer":"older");
        return ret;
    };

    QString xml = QDir(this->datapath(pod)).filePath(c_podcast_localxml);
    QFile xml_file(xml);
    QString json_str = QDir(datapath(pod)).filePath("pods_detail.json");
    QFile json_file(json_str);

    // if both data is not existed, download.
    if(json_file.exists() == false && xml_file.exists() == false) {
        this->podUpdate(pod);
        return false;
    }

    if(xml_file.exists()){
        // if json not existed, or not update.
        if(json_file.exists() == false || isNewer(xml_file, json_file))
        {
            bool ret = this->parsexml(pod);
            if(ret == false)
                return ret;
        }
    }

    // json should exist by now.
    return jsonload(&pod, json_str);
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
void Podcast::podUpdate(PodData &pod){
    updatexml(pod);
}

//load from local storage.
void Podcast::podLoad(PodData &pod){
    auto ret = load(pod);
    if(ret == false) return;
}

void Podcast::exportdlg() {}
void Podcast::write_opml(const QString &filename) {}

bool Podcast::updatexml(PodData &pod)
{
    if(pod.job_id != -1){
        auto status = DownloadManager::instance()->getJobStatus(pod.job_id);
        if(status == TASK_DOWNLOADING){
            binfo("some one want's to try download  a pod twice?");
            return false;
        } else {
            binfo("retry a failed xml download({})", pod.url.toStdString());
            binfo("last time the job ret is {}.", status);
            qDebug()<<status;
        }
    }
    auto res = QDir(this->datapath(pod)).filePath(c_podcast_localxml);
    pod.job_id = DownloadManager::instance()->addjob(pod.url, res);
    connect(DownloadManager::instance(), &DownloadManager::stateChanged, this,
            [&pod, this](int id, auto st){
                if(id == pod.job_id){
                    if(st == TASK_COMPLETE){
                        this->parsexml(pod);
                        if(d->detail->currentWidget() == d->detaillist){
                            if(pod.title == this->d->detaillist->current())
                                this->d->detaillist->refresh();
                        }
                    }
                }
            });
    return true;
}

bool Podcast::parsexml(PodData &pod) 
{
    QString xml = QDir(this->datapath(pod)).filePath(c_podcast_localxml);
    QFile xml_file(xml);
    auto parser = RssParser(&xml_file, &pod);
    bool ret = parser.parse();
    if(ret) {
        pod.location = Data::podcast_datapath(pod.title);
        save(pod);
    }
    return ret;
}
