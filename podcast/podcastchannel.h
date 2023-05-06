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
class PodcastChannel : public QObject {
  public:
    PodcastChannel(const QString &x, const QString &y, QObject *parent = nullptr)
        : QObject(parent) {
        title = x;
        url = y;
        location = Data::podcast_datapath(title);
    }
    bool isValid() { return !(title.isEmpty() || url.isEmpty()); }
    // TODO:
    // PodcastChannel& operator=(const PodData&) = default;
    ~PodcastChannel() = default;
    QString title;
    QString url;
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

    // parse downloaded xml to episocdlist.
    bool parserxml();

    // save from xml to json.
    bool save();
};

#endif
