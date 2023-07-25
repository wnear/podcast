#ifndef _POD_DATA_H
#define _POD_DATA_H

#include <QString>
#include <QDateTime>
#include <QDir>
#include <qpixmap.h>
#include "utils.h"
#include "global.h"
#include "episodedata.h"

constexpr char c_podcast_localxml[] = "podcast.xml";


// xml, json, xml is from rss, json is for local.
// TODO: use json to save deleted item at original rss.xml
//
//TODO: lastest episode.
class PodcastChannel : public QObject {
  public:
    PodcastChannel(const QString &title, const QString &url, QObject *parent = nullptr)
        : QObject(parent) {
        m_feedTitle = title;
        m_feedUrl = url;
        location = Data::podcastChannelDataPath(m_feedTitle);
    }
    bool isValid() { return !(m_feedTitle.isEmpty() || m_feedUrl.isEmpty()); }
    size_t size(){
        return episodes.size();
    }

    // TODO:
    // PodcastChannel& operator=(const PodData&) = default;
    ~PodcastChannel() = default;

    QString m_feedTitle;
    QString m_feedUrl;

    QString cover_url{};
    int job_id{-1};
    QString location;
    QString xmllocation;
    QDateTime lastEpisodeUpdate;
    QString language;
    int episodeCount;
    int episodeDuratinSum;
    QList<EpisodeData *> episodes;

    const QString xmlfmt = "podcast.xml";
    const QString jsonfmt = "pods_detail.json";
    const QString coverfmt{"cover.jpg"};

    const QString c_podcast_default_filename = "podcast.xml";

    QString datapath() const;
    QString coverfile() const;
    QString xmlfile() const;
    QString jsonfile() const;

    // will check both json and xml, check for existance and modification time.
    bool load();
    bool load_json();

    // download xml file from rss url.
    bool updatexml();
    bool parserxml();

    // save from xml to json.
    bool save();
};

#endif
