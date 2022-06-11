#include "episodewgt.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QDebug>
#include <QMenu>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include "player_engine.h"

#include <QFile>

#include "downloadmanager.h"

static int a = 0;
static QNetworkAccessManager net;

struct EpisodeWidgetPrivate {
    QLabel *title;
    QLabel *progress;
    QTextEdit *info;
};

namespace {
QString size_human(int size_byte){
    return QString("%1 MB").arg(size_byte*1.0/1024/1024,0, 'f', 2 );
}
QString percent(float val){
    return QString("[%1 %]").arg(val * 100,0, 'f', 1 );
}
QString percentStr(int cur, int total){
    return percent(cur*1.0/total);
}
}

void EpisodeWidget::setProgressText() {
    QString title = m_data->stateString();
    QString progress;
    int cur{m_data->currentSize()}, total{m_data->filesize};
    do {
        if(m_data->getState() == pd::MediaFileDownloading){
            if(total == 0){                         // filesize not have meaning.
                total += m_data->net_totalsize;     
                if(total)                           // filesize not have meaning ,but net_total has, still can save.
                    total += cur;
            }
            cur += m_data->net_cursize;
        } else {
        }

        if(total == 0){
            progress = size_human(cur);
        } else {
            progress = percentStr(cur,total);
        }
    }while(0);

    d->progress->setText(title + progress);
}

EpisodeWidget::EpisodeWidget(EpisodeData *data, QWidget *parent)
                            :QFrame(parent), id(a++),m_data(data)
{
    d = new EpisodeWidgetPrivate;
    d->title = new QLabel(this);
    d->progress = new QLabel("0%", this);
    setProgressText();
    d->info = new QTextEdit(this);

    auto *lay = new QVBoxLayout(this);
    lay->addWidget(d->title);
    lay->addWidget(d->progress);
    lay->addWidget(d->info);
    this->setLayout(lay);

    d->title->setText(m_data->title);
    d->info->setText(m_data->description);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this,
            &EpisodeWidget::onCustomContextMenuRequested);
    connect(DownloadManager::instance(), &DownloadManager::progress,
            this,  &EpisodeWidget::setProgressText);
}

QString EpisodeWidget::msg() const 
{
    return QString("%1").arg(id);
}


void EpisodeWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QString fileOndisk = m_data->location;
    auto dw = DownloadManager::instance();
    auto *player = PlayerEngine::instance();

    auto menu = new QMenu;
    menu->addAction("hello");
    menu->addAction("play on stream", [player, this](){
                        player->play(this->m_data->url);
                    });
    
    if(m_data->canPlay()) {
        menu->addAction("Play On disk", [player, this](){
                            auto file = this->m_data->location;
                            qDebug()<<"contexmenu, play: "<<file;
                            player->play(this->m_data->location);
                        });
    }

    if(m_data->canDownlad())
    { menu->addAction("start Download", [this](){ m_data->startDownload(); }); }

    if(m_data->canAbort())
    { menu->addAction("Abort Downloading", [this](){ m_data->abortDownload(); }); }

    menu->exec(mapToGlobal(pos));
}
