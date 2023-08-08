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
// TODO: lastest episode.
class PodcastChannel : public QObject {
    Q_OBJECT

  public:
    PodcastChannel(const QString &title, const QString &url, QObject *parent = nullptr);
    bool isValid() { return !(m_feedTitle.isEmpty() || m_feedUrl.isEmpty()); }
    size_t size() { return episodes.size(); }

    // TODO:
    // PodcastChannel& operator=(const PodData&) = default;
    ~PodcastChannel() = default;

    int channelID{-1};
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

    //TODO: refactor to batch add for view and db perfomance,
    //design:
    //`list<episodes*> m_neweps;`
    //`void prepareAdd(){m_neweps.clear();}`
    //`void addEpisode(ep){m_neweps.pushback(ep);}`
    //`void finnishAdd(){m_episodes.pushback();}`
    //
    //design 2:
    //register callback for prepareAdd and finishAdd.
    //
    //
    //NOTE: tricky, also a bug of previous impl(no appending ever happens):
    //episodes in xml and gui should be listed from new to old,
    //episodes in sql and ch.eps, in the other hand, should be old to new.
    //a order reversal should happen somewhere.
    void addEpisode(EpisodeData *ep);
    void finishUpdate(){
        emit channelUpdated();
        emit episodesUpdated();
    }

    // will check both json and xml, check for existance and modification time.
    bool load();
    bool load_json();

    // download xml file from rss url.
    bool updatexml();
    bool parserxml();

    // save from xml to json.
    bool save();

  signals:
    void channelUpdated();
    void episodesUpdated();
};

#endif
