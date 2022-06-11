#include "jsonop.h"
#include "poddata.h"
#include "episodedata.h"
#include "log.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

bool jsonload(PodData *entity, const QString &filepath)
{
    PodData &pod = *entity;
    QFile json_file(filepath);

    if(json_file.exists() == false){
        return false;
    }

    if(json_file.open(QIODevice::ReadOnly) == false){
        qDebug()<<"erro to open pod cofig for read: "<<pod.title;
        return false;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(json_file.readAll(), &err);
    if(doc.isNull()){
        qDebug()<<"not valid json file";
    }
    if(err.error != QJsonParseError::NoError){
        qDebug()<<"json parse error for load pod: "<<err.errorString();
        return false;
    }
    auto &&obj = doc.object();
    auto pod_ = obj["podinfo"].toObject();
    if(! pod_.isEmpty()){
        pod.title = pod_["title"].toString();
        pod.url = pod_["url"].toString();
        // if(pod.location.isEmpty())
        //     pod.location = datapath(pod);
    }
    auto eps = obj["episodes"].toArray();
    for(auto i: eps){
        EpisodeData *x = new EpisodeData;
        auto && obj = i.toObject();
        x->title = obj["title"].toString();
        //x->url = QUrl(obj["url"].toString());
        x->url = QUrl::fromEncoded(obj["url"].toString().toLatin1());
        x->cached = obj["cached"].toBool();
        x->location = obj["location"].toString();
        x->filesize = obj["filesize"].toInt();
        binfo("Get media file size is :{}", x->filesize);
        //if(x->location.isEmpty())
        x->location = QDir(pod.location).absoluteFilePath(x->url.fileName());
        x->updatetime = QDateTime::fromString(obj["updatetime"].toString()) ;
        x->duration = obj["duration"].toInt();
        pod.episodes.push_back(x);
    }
    return true;
}

bool jsonsave(PodData *entity, const QString &filepath)
{
    PodData &pod = *entity;
    auto file = filepath;
    QFile f(file);
    if(f.open(QIODevice::WriteOnly) == false){
        qDebug()<<"erro to open pod cofig for write: "<<pod.title;
        return false;
    }

    QJsonObject whole;
    whole["podinfo"] = QJsonObject{{"title", pod.title}, {"url", pod.url}};
    QJsonArray eps;
    for(auto e: pod.episodes) {
        // QJsonObject obj;
        // obj["title"] = e.title;
        // obj["url"] = e.url.toString();
        // obj["cached"] = e.cached;
        // obj["update"] = e.updatetime.toString();
        // obj["duration"] = e.duration;
        eps.push_back(QJsonObject{{"title", e->title},
                                {"cached", e->cached},
                                {"updatetime", e->updatetime.toString()},
                                {"duration", e->duration},
                                {"location", e->location},
                                {"filesize", e->filesize},
                                {"url", e->url.toString()},
                      });
        
    }
    whole["episodes"] = eps;
    QJsonDocument doc;
    doc.setObject(whole);
    f.write(doc.toJson());
    return true;
}
