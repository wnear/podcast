#include "episodewgt.h"
#include "log.h"

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
#include "utils.h"

static int a = 0;
static QNetworkAccessManager net;

struct EpisodeWidgetPrivate {
    QLabel *title;
    QLabel *progress;
    QLabel *duration;
    QLabel *size;
    QLabel *info;
};

namespace {
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
    binfo("progress: {} / {}", cur, total);
    d->progress->setText(title + progress);
}

EpisodeWidget::EpisodeWidget(EpisodeData *data, QWidget *parent)
                            :QFrame(parent), id(a++),m_data(data)
{
    this->setFrameStyle(QFrame::Raised);
    d = new EpisodeWidgetPrivate;
    d->title = new QLabel(this);
    d->progress = new QLabel("0%", this);
    d->duration = new QLabel(this);
    d->size = new QLabel(this);
    setProgressText();
    d->info = new QLabel(this);
    d->info->setWordWrap(true);

    auto *lay = new QVBoxLayout(this);
    lay->addWidget(d->title);

    auto *info = new QHBoxLayout();
    info->addWidget(d->progress);
    info->addWidget(d->duration);
    info->addWidget(d->size);
    lay->addLayout(info);

    lay->addWidget(d->info);
    this->setLayout(lay);

    d->title->setText(m_data->title);
    //TODO: show summary of first 100.
    auto fm = d->info->fontMetrics();
    auto description = fm.elidedText(m_data->description, Qt::ElideRight,fm.horizontalAdvance("a") * 200);
    d->info->setText(description);
    d->duration->setText(int2hms(m_data->duration));
    d->size->setText(size_human(m_data->filesize));
    

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this,
            &EpisodeWidget::onCustomContextMenuRequested);
    connect(m_data, &EpisodeData::fileChanged, 
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
    menu->addAction("info", [this](){
                        this->m_data->test();
                    });
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

    if(m_data->canDelete())
    { menu->addAction("delete media file", [this](){ m_data->deleteMediafile(); }); }

    menu->exec(mapToGlobal(pos));
}
