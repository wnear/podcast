#ifndef _EPISODE_DATE_H
#define _EPISODE_DATE_H

#include <QString>
#include <QDateTime>
#include <QUrl>

struct EpisodeData
{
        int jobid{-1};  // runtime value, to access the download status.
        QString title;
        QString author;
        QUrl url;
        int filesize;
        QString description;
        QDateTime updatetime;
        QString updatetime_str;
        int duration;

        QString location;
        bool cached;

        int currentSize() const;
        QString currentSize_str() const;
};

#endif
