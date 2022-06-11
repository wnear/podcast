#include "episodedata.h"
#include <QFileInfo>
#include "downloadmanager.h"

using namespace pd;


int EpisodeData::currentSize() const
{
    QFile f(location);
    if(location.isEmpty() || f.exists() == false)
        return -1;
    return QFileInfo(f).size();
}


QString EpisodeData::currentSize_str() const
{
    auto siz = currentSize();
    return QString("%1 MB").arg(siz/1024/1024);
}

pd::EpisodeState EpisodeData::getState() const {
    auto size = this->property("actualSize").toInt();
    if(state == pd::MediaFileUnknown){
        if(size == 0)
            return pd::MediaFileNone;
        else if(size < filesize)
            return pd::MediaFilePartially;
        else if(size == filesize)
            return pd::MediaFileComplete;
        else 
            return pd::MediaFilePartially; // filesize is set eror, may be can continue download, for safety.
            //assert(0); // should not step into here.
    } else {
        return state;
    }
}


QString EpisodeData::stateString()
{
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
        default:
            return "";
    }
}


void EpisodeData::startDownload()
{
    auto dw = DownloadManager::instance();
    auto start = this->property("actualSize").toInt();
    this->jobid = dw->addjob(this->url,
                             this->location, start);
    this->state = pd::MediaFileDownloading;
    connect(dw, &DownloadManager::stateChanged, 
            [this](auto t1, auto t2){
                if(t1 == this->jobid){
                    if(t2 == TASK_COMPLETE)
                        this->state = pd::MediaFileComplete;
                    if(t2 == TASK_NETERROR)
                        this->state = pd::MediaFileDownloadError;
                    if(t2 == TASK_COMPLETE)
                        this->state = pd::MediaFileComplete;
                }
            });
    connect(dw, &DownloadManager::progress, 
            [this](auto t1, auto t2, auto t3){
                if(t1 == this->jobid){
                    this->net_cursize = t2;
                    this->net_totalsize = t3;
                }
            });
    disconnect();
}

void EpisodeData::abortDownload()
{
    auto dw = DownloadManager::instance();
    dw->abort_job(this->jobid);
}
bool EpisodeData::canAbort()const {
    return this->getState() == pd::MediaFileDownloading;
}
bool EpisodeData::canPlay() const {
    return this->property("actualSize").toInt() > 0;
}
bool EpisodeData::canDownlad() const {
    bool ret =  this->property("actualSize").toInt() < this->filesize || this->filesize == 0;
    return ret;
}
