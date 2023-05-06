#include "downloadmanager.h"
#include "log.h"
#include "utils.h"
#include <QFile>
#include <QStringList>

class JobInfo {
  public:
    jobid_t id;
    jobstatus_t state;
    QString errorStr;
    QUrl url;
    QString dest;
    QByteArray data;
    QNetworkReply *reply;
    int cur;
    int total;
    QDateTime start_time;
    QDateTime end_time;
    QString speed{"unfinished"};
    // startTime.
    // endTime.

    void setProgress(int r, int t) {
        cur = r;
        total = t;
    }
    void appendData(QIODevice *d) { data.append(d->readAll()); }
    void save() {
        QFile f(dest);
        QString header = f.exists() ? "continue" : "fresh";

        do {
            if (data.size() == 0) {
                berror("download finish, size zero, early exit.");
                break;
            }

            if (dest.isEmpty()) {
                berror("mediafile location not specified");
                break;
            }
            // continue last download.
            if (f.exists() == false) {
                if (f.open(QIODevice::Append) == false) {
                    berror("error to open file for append write.");
                    break;
                }
            } else {
                if (f.open(QIODevice::WriteOnly) == false) {
                    berror("error to open file for fresh write.");
                    break;
                }
            }
            f.write(data);
            f.close();
            // TODO:
            // binfo("finish {}-download, file size is : {}", header, f.size());
            return;
        } while (0);
        // berror("error to download {}", url.toString());
        // berror(this->url.toString());
    }
};

DownloadManager *DownloadManager::ins = nullptr;

DownloadManager::DownloadManager(QObject *parent) : QObject(parent) {}
DownloadManager::~DownloadManager() {}

QPair<int, int> DownloadManager::getJobProgress(jobid_t id) {
    auto j = m_jobs[id];
    return {j->cur, j->total};
}

jobstatus_t DownloadManager::getJobStatus(jobid_t id) {
    if (m_jobs.keys().contains(id))
        return m_jobs[id]->state;
    else
        return NOTFOND;
}

// temp file??
jobid_t DownloadManager::addjob(QUrl url, const QString &dest, int start) {
    qDebug() << __FILE__ << __LINE__ << "add job for" << url.toString();
    qDebug() << __FILE__ << __LINE__ << "complete will be saved to: " << dest;

    QNetworkRequest req(url);
    if (start != 0) {
        qDebug() << "special downloading, continue from positon of: " << start;
        req.setRawHeader(QByteArray("Range"),
                         QString("bytes=%1-").arg(start).toLocal8Bit());
    }

    auto job = new JobInfo;
    job->id = m_cur;
    job->url = url;
    job->dest = dest;
    job->start_time = QDateTime::currentDateTime();
    job->state = TASK_DOWNLOADING;
    job->errorStr = "";

    auto *reply = m_net.get(req);
    job->reply = reply;
    // finish
    connect(reply, &QNetworkReply::finished, [this, job]() {
        job->state = TASK_COMPLETE;
        job->end_time = QDateTime::currentDateTime();
        int elpase = job->end_time.secsTo(job->start_time);
        if (elpase != 0) job->speed = QString("%1 kBps").arg(job->total / elpase / 1000);
        job->save();
        qDebug() << "network, download complete";
        emit stateChanged(job->id, job->state);
    });

    connect(reply, &QNetworkReply::errorOccurred, [this, reply, job](auto &&errorcode) {
        if (errorcode == QNetworkReply::OperationCanceledError)
            job->state = TASK_USR_ABORT;
        else
            job->state = TASK_NETERROR;
        job->end_time = QDateTime::currentDateTime();
        job->errorStr = reply->errorString();
        job->save();
        bwarn("Download have error: {}", job->errorStr);
        bwarn("job url: {}", job->url.toString());
        emit stateChanged(job->id, job->state);
    });
    connect(reply, &QNetworkReply::downloadProgress, [this, job](int cur, int total) {
        job->setProgress(cur, total);
        emit progress(job->id, cur, total);
    });
    connect(reply, &QNetworkReply::readyRead, [job, reply]() { job->appendData(reply); });

    m_jobs.insert(m_cur, job);
    m_cur += 1;
    return m_cur - 1;
}

QString DownloadManager::toString() {
    QStringList res;

    auto keys = m_jobs.keys();
    res.push_back("Job Detail:");

    res.push_back(QString("now has %1 jobs").arg(keys.length()));
    for (auto i = 0; i < keys.length(); i++) {
        res.push_back(QString("Job %1: ").arg(i));
        res.push_back(QString("[%1] -> [%2]")
                          .arg(m_jobs[keys[i]]->url.toString())
                          .arg(m_jobs[keys[i]]->dest));
        res.push_back(QString("progress: [%1/%2]")
                          .arg(m_jobs[keys[i]]->cur)
                          .arg(m_jobs[keys[i]]->total));
        res.push_back(QString("speed: [%1]").arg(m_jobs[keys[i]]->speed));
        res.push_back("===================");
    }
    return res.join(QChar::LineSeparator);
}

void DownloadManager::abort_job(jobid_t id) {
    auto *job = m_jobs.value(id);
    if (job != nullptr && job->state == TASK_DOWNLOADING) {
        job->reply->abort();
    }
}
