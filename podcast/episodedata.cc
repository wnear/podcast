#include "episodedata.h"
#include <QFileInfo>
#include "downloadmanager.h"
#include "log.h"

using namespace pd;

EpisodeData::EpisodeData(QObject *parent) : QObject(parent) { calculateCurrentSize(); }

void EpisodeData::test() {
    binfo("info: size {}", this->currentSize());
    binfo("info: media location {}", this->location);
    binfo("info: state {}", this->stateString());
}

void EpisodeData::calculateCurrentSize() {
    QFile f(location);

    if (location.isEmpty() || f.exists() == false) actualSize = 0;
    actualSize = QFileInfo(f).size();
}

int EpisodeData::currentSize() const { return actualSize; }

QString EpisodeData::currentSize_str() const {
    auto siz = currentSize();
    return QString("%1 MB").arg(siz / 1024 / 1024);
}

pd::EpisodeState EpisodeData::getState() const {
    if (state == pd::MediaFileUnknown) {
        auto size = this->property("actualSize").toInt();
        if (size == 0)
            return pd::MediaFileNone;
        else if (size < filesize)
            return pd::MediaFilePartially;
        else if (size == filesize)
            return pd::MediaFileComplete;
        else
            return pd::MediaFilePartially;  // filesize is set eror, may be can continue
                                            // download, for safety.
                                            // assert(0); // should not step into here.
    } else {
        return state;
    }
}

QString EpisodeData::stateString() {
    switch (getState()) {
        case MediaFileUnknown:
            return QStringLiteral("--");
        case MediaFileDownloading:
            return QStringLiteral("Downoading");
        case MediaFileDownloadError:
            return QStringLiteral("downloading error");
        case MediaFileDownloadFinish:
            return QStringLiteral("Complete");
        case MediaFileNone:
            return QStringLiteral("No local");
        case MediaFilePartially:
            return QStringLiteral("Partially");
        case MediaFileDownloadPause:
            return QStringLiteral("Pause");
        default:
            return "";
    }
}

void EpisodeData::startDownload() {
    auto dw = DownloadManager::instance();
    auto start = this->property("actualSize").toInt();
    this->jobid = dw->addjob(this->url, this->location, start);
    this->state = pd::MediaFileDownloading;
    connect(dw, &DownloadManager::stateChanged, [this](auto t1, auto t2) {
        if (t1 == this->jobid) {
            binfo("download manager state changed for this episode");
            calculateCurrentSize();
            if (t2 == TASK_COMPLETE) this->state = pd::MediaFileComplete;
            if (t2 == TASK_NETERROR) this->state = pd::MediaFileDownloadError;
            if (t2 == TASK_USR_ABORT) this->state = pd::MediaFileDownloadPause;
            binfo("after statechange, file size is :{}", this->currentSize());
            emit fileChanged();
        }
    });
    connect(dw, &DownloadManager::progress, [this](auto t1, auto t2, auto t3) {
        if (t1 == this->jobid) {
            binfo("download have a progress");
            this->net_cursize = t2;
            this->net_totalsize = t3;
            emit fileChanged();
        }
    });
}

void EpisodeData::abortDownload() {
    auto dw = DownloadManager::instance();
    dw->abort_job(this->jobid);
}
bool EpisodeData::canAbort() const {
    return this->getState() == pd::MediaFileDownloading;
}
bool EpisodeData::canPlay() const { return this->property("actualSize").toInt() > 0; }
bool EpisodeData::canDownlad() const {
    bool ret =
        this->property("actualSize").toInt() < this->filesize || this->filesize == 0;
    return ret;
}

bool EpisodeData::canDelete() const { return actualSize > 0; }

bool EpisodeData::deleteMediafile() {
    bool ret = QFile(this->location).remove();
    if (ret) {
        calculateCurrentSize();
        emit fileChanged();
    }
    return ret;
}
QVariant EpisodeData::getByTreeVolume(TreeColumn) const { return QVariant(); }
