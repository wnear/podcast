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

#include "podcast.h"

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
};

Podcast::Podcast(QWidget *parent): QWidget(parent)
{
    d = new Private;
    load();
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
                qDebug()<<"url is "<<url;
            });
}

void Podcast::save()
{
    auto path = this->datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile f(podsfile);
    QJsonArray whole;
    for(auto i: m_pods){
        whole.push_back(QJsonObject{{"title", i.title}, {"url", i.url}});
    }
    QJsonDocument doc;
    doc.setArray(whole);
    if(f.open(QIODevice::WriteOnly) == false){
        qDebug()<<"Err to open for write";
    }
    f.write(doc.toJson());
}

void Podcast::load()
{
    auto path = this->datapath();
    auto podsfile = QDir(path).filePath("subscription.json");
    QFile pods(podsfile);
    if(pods.exists() == false || pods.open(QIODevice::ReadOnly) == false){
        qDebug()<<"fail to open";
        return;
    }
    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(pods.readAll(), &err);
    if(err.error != QJsonParseError::NoError) {
        qDebug()<<"Json parse error: "<<err.errorString();
    }
    for(auto i: doc.array()) {
        auto item = i.toObject();
        qDebug()<<item.value("title").toString();
        qDebug()<<item.value("url").toString();
        m_pods.push_back(PodData(item.value("title").toString(), item.value("url").toString()));
    }
}


QWidget *Podcast::detail() const 
{ return d->detail; }


QString Podcast::datapath() const
{
    QString app_datapath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    return ensureDirExist(app_datapath, "podcast");
}

QString Podcast::datapath_pod(const QString &name) const
{ return ensureDirExist(datapath(), name); }

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
    auto alreadyHave = [this](const QString &title){
        auto i = std::find_if(m_pods.begin(), m_pods.end(), [title](auto && data){
                                  return data.title == title;
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
                if(alreadyHave(x.title)){
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

void Podcast::exportdlg() {}
void Podcast::write_opml(const QString &filename) {}
