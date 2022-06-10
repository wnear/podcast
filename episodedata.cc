#include "episodedata.h"
#include <QFileInfo>

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
    if(state == pd::MediaFileUnknown){
        if(currentSize() == 0)
            return pd::MediaFileNone;
        else if(currentSize() < filesize)
            return pd::MediaFilePartially;
        else if(currentSize() == filesize)
            return pd::MediaFileComplete;
        else 
            assert(0); // should not step into here.
    } else {
        return state;
    }
}
