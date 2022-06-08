#include "player_engine.h"
#include <QMediaPlayer>
#include <QDebug>

PlayerEngine::PlayerEngine(QObject* parent)
    :QObject(parent)
{
    player = new QMediaPlayer(this);
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
