#ifndef _POD_DATA_H
#define _POD_DATA_H

#include <QString>
#include <QDateTime>
#include "episodedata.h"

constexpr char c_podcast_localxml[] = "podcast.xml";

struct PodData
{

        PodData(const QString &x, const QString &y){
            title = x;
            url  = y;
        }
        bool isValid(){
            return !(title.isEmpty() || url.isEmpty() );
        }
        QString title;
        QString url;
        int job_id{-1};
        QString location;
        QString xmllocation;
        QDateTime lastEpisodeUpdate;
        QString language;
        int episodeCount;
        int episodeDuratinSum;
        QList<EpisodeData* > episodes;

        const QString c_podcast_default_filename = "podcast.xml";
};

#endif
