#ifndef _DOWNLOAD_MANAGER_H
#define _DOWNLOAD_MANAGER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

/**
configurable:
- download position.
- download queue, max number.
- max speed.

show
- download state.
- download speed.
    */
class DownloadManager: public QObject
{
        Q_OBJECT

public:
    DownloadManager(QObject *parent = nullptr){}
    ~DownloadManager(){}

private:

};



#endif




