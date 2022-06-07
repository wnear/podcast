#include <QFileDialog>
#include <QDomDocument>
#include <QDebug>

#include <QFile>
#include <QStandardPaths>


#include <QListView>
#include <QVBoxLayout>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include "podcast.h"
#include "rssparser.h"

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
    QListView *detail;
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
    d->podsmodel = new PodModel(m_pods, this);
    d->list->setModel(d->podsmodel);
    d->detail = new QListView(this);
    lay->addWidget(d->list);

    connect(d->list, &QListView::clicked, [this](auto &&idx){
                // title/ url ==> feed ==> detailview ==> detailview update.
                auto model = qobject_cast<PodModel *>(d->list->model());
                auto url = model->data(idx, PodModel::UrlRole).toString();
                int row = idx.row();
                qDebug()<<"url is "<<url;
                podLoad(m_pods[row]);
                podUpdate(m_pods[row]);
                
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
        whole.push_back(QJsonObject{{"title", i.title}, {"url", i.url}});
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
        qDebug()<<item.value("title").toString();
        qDebug()<<item.value("url").toString();
        m_pods.push_back(PodData(item.value("title").toString(),
                                 item.value("url").toString()));
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

QString Podcast::getConfigDirectory(PodData &pod) const
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
                                  return p.url == now.url;
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
            PodData x(elnode.attribute("text", ""), 
                      elnode.attribute("xmlUrl", ""));
            if(x.isValid()) {
                if(alreadyHave(x)){
                }else {
                    added += 1;
                    m_pods.push_back(x);
                }
            }
        }
    }
    qDebug()<< QString("result: %1 of %2 is added").arg(added).arg(sum);
    d->podsmodel->resetData(m_pods);
    d->list->reset();

    save();
}

void Podcast::savepod(QString title, QString url) {
    m_pods.push_back({title, url});
}

bool Podcast::save(PodData &pod){
    auto file = QDir(getConfigDirectory(pod)).filePath("pods_detail.json");
    QFile f(file);
    if(f.open(QIODevice::WriteOnly) == false){
        qDebug()<<"erro to open pod cofig for write: "<<pod.title;
        return false;
    }

    QJsonObject whole;
    whole["podinfo"] = QJsonObject{{"title", pod.title}, {"url", pod.url}};
    QJsonArray eps;
    for(auto e: pod.episodes)
        eps.push_back(QJsonObject{{"title", e.title},
                                {"url", e.url},
                                {"cached", e.cached},
                                {"updatetime", e.updatetime.toString()},
                                {"duration", e.duration},
                      });
    whole["episodes"] = eps;
    QJsonDocument doc;
    doc.setObject(whole);
    f.write(doc.toJson());
    return true;
}

bool Podcast::load(PodData &pod){
    auto file = QDir(getConfigDirectory(pod)).filePath("pods_detail.json");
    QFile f(file);
    if(f.exists() == false || f.open(QIODevice::ReadOnly) == false){
        qDebug()<<"erro to open pod cofig for read: "<<pod.title;
        return false;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(f.readAll(), &err);
    if(err.error != QJsonParseError::NoError){
        qDebug()<<"json parse error for load pod: "<<err.errorString();
        return false;
    }
    auto &&obj = doc.object();
    //auto pod_ = obj["podinfo"];
    auto eps = obj["episodes"].toArray();
    for(auto i: eps){
        EpisodeData x;
        auto && obj = i.toObject();
        x.title = obj["title"].toString();
        x.url = obj["url"].toString();
        x.cached = obj["cached"].toBool();
        x.updatetime = QDateTime::fromString(obj["updatetime"].toString()) ;
        x.duration = obj["duration"].toInt();
    }
    return true;
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
    qDebug()<<QString("request begin for %1[%2]").arg(pod.title).arg(pod.url);
    QNetworkRequest req;
    req.setUrl(pod.url);
    auto *reply = d->net->get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::errorOccurred, [reply](){
                qDebug()<<"request error: "<<reply->errorString();
            });
    connect(reply, &QNetworkReply::downloadProgress, [](auto &&recv, auto &&total){
                qDebug()<<QString("request, progress: [%1/%2]").arg(recv).arg(total);
            });
    loop.exec();
    qDebug()<<"request finish";
    
    auto parser = RssParser(reply, &pod);
    auto ret = parser.parse();

    //save(pod);
}

//load from local storage.
void Podcast::podLoad(PodData &pod){
    auto ret = load(pod);
    if(ret == false) return;
}

void Podcast::exportdlg() {}
void Podcast::write_opml(const QString &filename) {}
