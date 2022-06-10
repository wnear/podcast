#ifndef _EPISODE_DATE_H
#define _EPISODE_DATE_H

#include <QString>
#include <QDateTime>
#include <QUrl>

namespace pd{
enum EpisodeState{
    MediaFileUnknown,

    MediaFileDownloading,
    MediaFileDownloadError,
    MediaFileDownloadFinish,
    
    MediaFileNone,
    MediaFilePartially,
    MediaFileComplete,
};
}

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

        pd::EpisodeState getState() const;
        void setState(pd::EpisodeState s){
            state = s;
        }
        int currentSize() const;
        QString currentSize_str() const;
    private:
        /**
        *
        * original state is None, the returned state is calculated by size checking, 
        * after download, state is changed manually, indicate downloading error if existed, 
        * carrying more meaning.
        */
        pd::EpisodeState state{pd::MediaFileUnknown};  

};

#endif
