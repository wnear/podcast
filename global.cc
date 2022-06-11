#include "global.h"

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

    d->podcast = QDir(d->app).absoluteFilePath(d->app);
    if(QDir(d->podcast).exists() == false){
        assert(QDir(d->app).mkpath(d->podcast));
    }

    d->xmlname = "podcast.xml";
    d->jsonname= "pod_detail.json";
}

QString Data::app_datapath()
{ return d->app; }

QString Data::podcast_datapath()
{ return d->podcast; }
