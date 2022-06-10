#include "player_engine.h"
#include <QMediaPlayer>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

static QMutex creatMutex;

PlayerEngine *PlayerEngine::ins = nullptr;
PlayerEngine *PlayerEngine::instance(QObject *parent) {
    if (ins == nullptr) {
        QMutexLocker lock(&creatMutex);
        ins = new PlayerEngine(parent);
    }
    return ins;
}

PlayerEngine::PlayerEngine(QObject* parent)
    :QObject(parent)
{
    player = new QMediaPlayer(this);
    connect(player, &QMediaPlayer::positionChanged,
            this, &PlayerEngine::positionChanged);
    connect(player, &QMediaPlayer::positionChanged,
            this, [this](int pos){
                if(this->duration() == 0)
                    return;
                emit progressChanged(pos*100/this->duration());
            });
}

void PlayerEngine::play(const QString &url)
{
    qDebug()<<"local file";
    this->play(QUrl::fromLocalFile(url));
}

void PlayerEngine::play(QUrl url){
    qDebug()<<"trying to play: "<< url.toString();
    player->stop();
    player->setMedia(url);
    player->setVolume(100);
    player->play();
}

void PlayerEngine::pause(){
    player->pause();
}

void PlayerEngine::stop(){
    player->stop();
}

void PlayerEngine::setPosition(int pos)
{
    player->setPosition(pos);
}

void PlayerEngine::setProgress(int percentage)
{
    player->setPosition(this->duration() * percentage / 100);
}

int PlayerEngine::duration() const
{
    return player->duration();
}
