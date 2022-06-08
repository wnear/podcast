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


static int a = 0;
static QNetworkAccessManager net;
static PlayerEngine player;

EpisodeWidget::EpisodeWidget(EpisodeData &data, QWidget *parent):QFrame(parent), id(a++),m_data(data) 
{
    auto *lay = new QVBoxLayout(this);
    lay->addWidget(new QLabel(QString("Episode: %1").arg(m_data.title)));
    lay->addWidget(new QTextEdit(QString("Detail: %1 ").arg(m_data.description)));
    this->setLayout(lay);

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

    auto menu = new QMenu;
    menu->addAction("hello");
    menu->addAction("play on stream", [this](){
                        player.play(this->m_data.url);
                    });
    if(! fileOndisk.isEmpty() && QFile(fileOndisk).exists())
    {
        menu->addAction("Play On disk", [](){});
    }


    menu->exec(mapToGlobal(pos));
}
