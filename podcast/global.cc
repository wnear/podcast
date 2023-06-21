#include "global.h"
#include "log.h"

#include <QStandardPaths>
#include <QDir>

class DataPrivate {
  public:
    QString appdata_rootdir;
    QString podcast_rootdir;
    QString subsonic;
    QString xmlname;
    QString jsonname;
};

DataPrivate* Data::d = nullptr;
void Data::init() {
    d = new DataPrivate;
    d->appdata_rootdir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    assert(!d->appdata_rootdir.isEmpty());
    if (QDir(d->appdata_rootdir).exists() == false) {
        QDir().mkpath(d->appdata_rootdir);
    }

    d->podcast_rootdir = QDir(d->appdata_rootdir).absoluteFilePath("podcast");
    if (QDir(d->podcast_rootdir).exists() == false) {
        assert(QDir(d->appdata_rootdir).mkpath(d->podcast_rootdir));
    }

    d->xmlname = "podcast.xml";
    d->jsonname = "pod_detail.json";

    binfo("app datapath: {}", Data::app_datapath());
    binfo("podcast datapath: {}", Data::podcast_datapath());
}

QString Data::app_datapath() { return d->appdata_rootdir; }

QString Data::podcast_datapath() { return d->podcast_rootdir; }

/* TODO:
 set datapath for podcast_xx in cofiguration.
 if not found, get the default path.
*/
QString Data::podcastChannelDataPath(const QString& title) {
    auto parentdir = d->podcast_rootdir;
    auto path = QDir(parentdir).absoluteFilePath(title);
    QFileInfo f(path);
    if (!f.exists()) {
        QDir{}.mkpath(path);
    } else {
        assert(f.isDir());
    }

    return path;
}
