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
static PlayerEngine player;

struct EpisodeWidgetPrivate {
    QLabel *title;
    QLabel *progress;
    QTextEdit *info;
};

EpisodeWidget::EpisodeWidget(EpisodeData &data, QWidget *parent):QFrame(parent), id(a++),m_data(data) 
{
    d = new EpisodeWidgetPrivate;
    d->title = new QLabel(this);
    d->progress = new QLabel("0%", this);
    d->info = new QTextEdit(this);

    auto *lay = new QVBoxLayout(this);
    lay->addWidget(d->title);
    lay->addWidget(d->progress);
    lay->addWidget(d->info);
    this->setLayout(lay);

    d->title->setText(m_data.title);
    d->info->setText(m_data.description);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &EpisodeWidget::onCustomContextMenuRequested);
}

QString EpisodeWidget::msg() const 
{
    return QString("%1").arg(id);
}


void EpisodeWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QString fileOndisk = m_data.location;
    auto dwld = DownloadManager::instance();

    auto menu = new QMenu;
    menu->addAction("hello");
    menu->addAction("play on stream", [this](){
                        player.play(this->m_data.url);
                    });
    if(! fileOndisk.isEmpty() && QFile(fileOndisk).exists())
    {
        menu->addAction("Play On disk", [this](){
                        player.play(this->m_data.location);
                        });
    } else {
        menu->addAction("Download", [this, dwld](){
                            m_data.jobid = dwld->addjob(m_data.url, m_data.location);
                        });
    }
    /** check download state **/
    


    menu->exec(mapToGlobal(pos));
}
