#include "global.h"
#include "log.h"

#include <QStandardPaths>
#include <QDir>

class DataPrivate {
public:
    QString app;
    QString podcast;
    QString subsonic;
    QString xmlname;
    QString jsonname;
    
};

DataPrivate* Data::d = nullptr;
void Data::init (){
    d = new DataPrivate;
    d->app = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    
    assert(!d->app.isEmpty());
    if(QDir(d->app).exists() == false){
        QDir().mkpath(d->app);
    }

    d->podcast = QDir(d->app).absoluteFilePath("podcast");
    if(QDir(d->podcast).exists() == false){
        assert(QDir(d->app).mkpath(d->podcast));
    }

    d->xmlname = "podcast.xml";
    d->jsonname= "pod_detail.json";

    binfo("app datapath: {}", Data::app_datapath());
    binfo("podcast datapath: {}", Data::podcast_datapath());
}

QString Data::app_datapath()
{ return d->app; }

QString Data::podcast_datapath()
{ return d->podcast; }

/* TODO:
 set datapath for podcast_xx in cofiguration.
 if not found, get the default path.
*/
QString Data::podcast_datapath(const QString& title) 
{
    auto path = QDir(Data::podcast_datapath()).absoluteFilePath(title);
    QFileInfo f(path);
    if(!f.exists()){
        QDir{}.mkpath(path);
    } else {
        assert(f.isDir());
    }

    return path;
}
