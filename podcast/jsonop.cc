#include "jsonop.h"
#include "podcastchannel.h"
#include "episodedata.h"
#include "log.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

bool jsonload(PodcastChannel *entity, const QString &filepath) {
    binfo("{} enter", __func__);
    binfo("filepath: {}", filepath.toStdString());
    PodcastChannel &pod = *entity;
    QFile json_file(filepath);

    if (json_file.exists() == false) {
        return false;
    }

    if (json_file.open(QIODevice::ReadOnly) == false) {
        qDebug() << "erro to open pod cofig for read: " << pod.m_feedTitle;
        return false;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(json_file.readAll(), &err);
    if (doc.isNull()) {
        qDebug() << "not valid json file";
    }
    if (err.error != QJsonParseError::NoError) {
        qDebug() << "json parse error for load pod: " << err.errorString();
        return false;
    }
    auto &&obj = doc.object();
    auto pod_ = obj["podinfo"].toObject();
    if (!pod_.isEmpty()) {
        pod.m_feedTitle = pod_["title"].toString();
        pod.m_feedUrl = pod_["url"].toString();
        // if(pod.location.isEmpty())
        //     pod.location = datapath(pod);
    }
    auto eps = obj["episodes"].toArray();
    pod.episodes.clear();
    for (auto i : eps) {
        EpisodeData *x = new EpisodeData;
        auto &&obj = i.toObject();
        x->title = obj["title"].toString();
        // x->url = QUrl(obj["url"].toString());
        x->url = QUrl::fromEncoded(obj["url"].toString().toLatin1());
        x->cached = obj["cached"].toBool();
        x->location = obj["location"].toString();
        x->description = obj["description"].toString();
        if (QFile(x->location).exists())
            binfo("downloaded episode, media file location: {}", x->location);
        x->filesize = obj["filesize"].toInt();
        x->updatetime = QDateTime::fromString(obj["updatetime"].toString());
        x->duration = obj["duration"].toInt();
        // TODO: episode data init should be in one loadFromJsoon,
        x->calculateCurrentSize();
        pod.episodes.push_back(x);
    }
    return true;
}

bool jsonsave(PodcastChannel *entity, const QString &filepath) {
    binfo("json save.");
    binfo("filepath: {}", filepath.toStdString());
    PodcastChannel &pod = *entity;
    binfo("test one, pod title: {}", pod.title);
    binfo("test one, pod location: {}", pod.location);
    binfo("test one, filesize: {}", pod.episodes[0]->filesize);
    binfo("test one, mediafile location: {}", pod.episodes[0]->location);
    assert(!pod.location.isEmpty());
    assert(QDir(pod.location).exists());
    auto file = filepath;
    QFile f(file);
    if (f.open(QIODevice::WriteOnly) == false) {
        qDebug() << "erro to open pod cofig for write: " << pod.m_feedTitle;
        return false;
    }

    QJsonObject whole;
    whole["podinfo"] = QJsonObject{{"title", pod.m_feedTitle}, {"url", pod.m_feedUrl}};
    QJsonArray eps;
    for (auto e : pod.episodes) {
        // QJsonObject obj;
        // obj["title"] = e.title;
        // obj["url"] = e.url.toString();
        // obj["cached"] = e.cached;
        // obj["update"] = e.updatetime.toString();
        // obj["duration"] = e.duration;
        e->location = QDir(pod.location).absoluteFilePath(e->location);
        eps.push_back(QJsonObject{
            {"title", e->title},
            {"description", e->description},
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
    binfo("ending");
    return true;
}
