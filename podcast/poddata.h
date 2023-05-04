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

class PodcastChannel: public QObject
{
    public:
        PodcastChannel(const QString &x, const QString &y, QObject *parent = nullptr)
            :QObject(parent)
        {
            title = x;
            url  = y;
            location = Data::podcast_datapath(title);
        }
        bool isValid(){
            return !(title.isEmpty() || url.isEmpty() );
        }
        //TODO:
        //PodcastChannel& operator=(const PodData&) = default;
        ~PodcastChannel() = default;
        QString title;
        QString url;
        QString cover_url{};
        const QString cover{"cover.jpg"};
        int job_id{-1};
        QString location;
        QString xmllocation;
        QDateTime lastEpisodeUpdate;
        QString language;
        int episodeCount;
        int episodeDuratinSum;
        QList<EpisodeData* > episodes;

        const QString c_podcast_default_filename = "podcast.xml";
        QString coverfile () const {
            return QDir(location).absoluteFilePath(cover);
        }
};

#endif
