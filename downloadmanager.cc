#include "downloadmanager.h"
#include <QFile>
#include <QStringList>

class JobInfo {
public:
    jobid_t id;
    status_t status;
    QString errorStr;
    QUrl url;
    QString dest;
    QByteArray data;
    int cur;
    int total;
    QDateTime start_time;
    QDateTime end_time;
    QString speed{"unfinished"};
    //startTime.
    //endTime.

    void setProgress(int r, int t){
        cur = r;
        total = t;
    }
    void appendData(QIODevice *d){
        data.append(d->readAll());
    }
    void save() {
        qDebug()<<"job save download";
        QFile f(dest);
        //assert(f.exists() == false);
        if(this->dest.isEmpty() || f.open(QIODevice::WriteOnly) == false){
            qDebug()<<"error to save";
            return;
        }
        f.write(data);
        qDebug()<<"job save download, after";
    }
};

DownloadManager*  DownloadManager::ins = nullptr;

DownloadManager::DownloadManager(QObject *parent) :QObject(parent) { }

QPair<int, int> DownloadManager::getJobProgress(jobid_t id){
    auto j = m_jobs[id];
    return {j->cur, j->total};
}


status_t DownloadManager::getJobStatus(jobid_t id){
    if(m_jobs.keys().contains(id))
        return m_jobs[id]->status;
    else 
        return NOTFOND;
}

// temp file??
jobid_t DownloadManager::addjob(QUrl url, const QString &dest)
{
    QNetworkRequest req(url);
    auto job = new JobInfo;
    job->id = m_cur;
    job->url = url;
    job->dest = dest;
    job->start_time = QDateTime::currentDateTime();
    job->status = DOING;
    job->errorStr = "";

    auto *reply = m_net.get(req);
    //finish
    connect(reply, &QNetworkReply::finished, [this, job](){
                job->status = COMPLETE;
                job->end_time = QDateTime::currentDateTime();
                int elpase = job->end_time.secsTo(job->start_time);
                if(elpase !=0)
                    job->speed =QString("%1 kBps").arg(job->total/elpase/1000) ;
                job->save();
                qDebug()<<"network, download complete";
                emit stateChanged(job->id, job->status);
            });

    connect(reply, &QNetworkReply::errorOccurred, [this, reply, job](){
                job->status = ERR;
                job->end_time = QDateTime::currentDateTime();
                job->errorStr = reply->errorString();
                qDebug()<<"network request error: "<<job->errorStr;
                emit stateChanged(job->id, job->status);
            });
    connect(reply, &QNetworkReply::downloadProgress, [this, job](int cur, int total){
                job->setProgress(cur, total);
                emit progress(job->id, cur, total);
            });
    connect(reply, &QNetworkReply::readyRead, [job, reply](){
                job->appendData(reply);
            });

    m_jobs.insert(m_cur, job);
    m_cur += 1;
    return m_cur-1;
}


QString DownloadManager::toString()
{
    QStringList res;

    auto keys = m_jobs.keys();
    res.push_back("Job Detail:");

    res.push_back(QString("now has %1 jobs").arg(keys.length()));
    for(auto i = 0; i< keys.length(); i++){
        res.push_back( QString("Job %1: ").arg(i));
        res.push_back( QString("[%1] -> [%2]")
                      .arg(m_jobs[keys[i]]->url.toString())
                      .arg(m_jobs[keys[i]]->dest)
                      );
        res.push_back( QString("progress: [%1/%2]")
                      .arg(m_jobs[keys[i]]->cur)
                      .arg(m_jobs[keys[i]]->total)
                      );
        res.push_back( QString("speed: [%1]")
                      .arg(m_jobs[keys[i]]->speed)
                      );
        res.push_back("===================");
    }
    return res.join(QChar::LineSeparator);
    
}
