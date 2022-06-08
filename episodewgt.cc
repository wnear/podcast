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

struct EpisodeWidget::Private {
    QLabel *title;
    QLabel *progress;
    QTextEdit *info;
};

EpisodeWidget::EpisodeWidget(EpisodeData &data, QWidget *parent):QFrame(parent), id(a++),m_data(data) 
{
    d = new Private;
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
        menu->addAction("Download", [this](){
                            QNetworkRequest req;
                            req.setUrl(m_data.url);
                            auto *reply = net.get(req);
                            QEventLoop loop;
                            connect(reply, &QNetworkReply::finished, [reply, this](){
                                        QFile f(m_data.location);
                                        f.open(QIODevice::WriteOnly);
                                        f.write(reply->readAll());
                                    });
                            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                            connect(reply, &QNetworkReply::errorOccurred, [reply](){
                                        qDebug()<<"request error: "<<reply->errorString();
                                    });
                            connect(reply, &QNetworkReply::downloadProgress, [this](auto &&recv, auto &&total){
                                        qDebug()<<QString("request, progress: [%1/%2]").arg(recv).arg(total);
                                        d->progress->setText(QString("[%1%]").arg(recv*100/total ));
                                    });
                            loop.exec();
                        });
    }


    menu->exec(mapToGlobal(pos));
}
