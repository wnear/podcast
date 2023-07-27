#ifndef _EPISODE_DATE_H
#define _EPISODE_DATE_H

#include <QString>
#include <QDateTime>
#include <QUrl>
#include <QObject>

#include "episode_treemodel.h"

namespace pd {
enum EpisodeState {
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

class EpisodeData : public QObject {
    Q_OBJECT
    Q_PROPERTY(int actualSize READ currentSize)

  public:
    struct bookmark_t {
        int position;
        QString title;
    };

    EpisodeData(QObject *parent = nullptr);
    ~EpisodeData() = default;
    int jobid{-1};  // runtime value, to access the download status.
    int id;         // id is a db key can be used across many table,
                    // like the play-history table, the bookmark/notes table.
    QString title;
    QString author;
    QUrl url;
    int net_cursize{0};    // During downloading, current download bytesize.
    int net_totalsize{0};  // During downloading, bytesize need to download reported from
                           // Network, not very useful if not a fresh download.

    int actualSize;   // mediafile's current size, of the disk file, useful if downloaded partially.
    int filesize{0};  // mediafile full filesize, parsed from the xml description.
    QString description;
    QDateTime updatetime;
    QString updatetime_str;

    QList<bookmark_t> bookmarks;
    QString notes;

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

    QVariant getByTreeVolume(TreeColumn) const;
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

// qRegisterMetaType<EpisodeData>("EpisodeData");

#endif
