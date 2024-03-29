#ifndef _DOWNLOAD_MANAGER_H
#define _DOWNLOAD_MANAGER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QString>

/**
configurable:
- download position.
- download queue, max number.
- max speed.

show
- download state.
- download speed.
    */

using jobid_t = int;

enum jobstatus_t {
    TASK_BUFFER = 0,
    TASK_DOWNLOADING,
    TASK_NETERROR,
    TASK_USR_ABORT,
    TASK_COMPLETE,
    NOTFOND,
};

class DownloadManager : public QObject {
    Q_OBJECT

  public:
    static DownloadManager *instance(QObject *parent = nullptr) {
        if (ins == nullptr) ins = new DownloadManager(parent);
        return ins;
    }
    DownloadManager(QObject *parent = nullptr);
    bool isDownloading(jobid_t);
    jobid_t addjob(QUrl url, const QString &dest = "", int start = 0);
    void abort_job(jobid_t);
    void rmove_job(jobid_t) {}
    QPair<int, int> getJobProgress(jobid_t);
    jobstatus_t getJobStatus(jobid_t);
    QString toString();

    ~DownloadManager();

  signals:
    void progress(jobid_t, int a, int b);
    void stateChanged(jobid_t, jobstatus_t);

  private:
    static DownloadManager *ins;
    QNetworkAccessManager m_net;
    jobid_t m_cur{0};
    QMap<jobid_t, class JobInfo *> m_jobs;
};

#endif
