#ifndef _EPISODE_DATE_H
#define _EPISODE_DATE_H

#include <QString>
#include <QDateTime>
#include <QUrl>
#include <QObject>

namespace pd{
enum EpisodeState{
    MediaFileUnknown,

    MediaFileDownloading,
    MediaFileDownloadError,
    MediaFileDownloadFinish,
    MediaFileDownloadPause,
    
    MediaFileNone,
    MediaFilePartially,
    MediaFileComplete,
};


}

class EpisodeData : public QObject
{
        Q_OBJECT
        Q_PROPERTY( int actualSize READ currentSize)

        public:
        EpisodeData(QObject *parent = nullptr);
        ~EpisodeData() = default;
        int jobid{-1};  // runtime value, to access the download status.
        QString title;
        QString author;
        QUrl url;
        int net_cursize{0};  // During downloading, current download bytesize.
        int net_totalsize{
            0};  // During downloading, bytesize need to download reported from Network, not very useful if not a fresh download.

        int actualSize;   // media file current size of the disk file.
        int filesize{0};  // media file filesize, parsed from the xml description.
        QString description;
        QDateTime updatetime;
        QString updatetime_str;

        QDateTime releaseTime;
        QDateTime downloadTime;

        int duration;

        QString location;
        bool cached;

        QString stateString();
        pd::EpisodeState getState() const;
        void setState(pd::EpisodeState s) { state = s; }
        void calculateCurrentSize();
        int currentSize() const;
        QString currentSize_str() const;

        bool canPlay() const;

        bool canDownlad() const;
        void startDownload();

        bool canAbort() const;
        void abortDownload();
        bool canDelete() const;
        bool deleteMediafile();

        void test();

        signals:
            void fileChanged();
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
