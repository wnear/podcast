#include "player_engine.h"
#include "log.h"

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
    connect(player, &QMediaPlayer::durationChanged,
            this, &PlayerEngine::setDuration);
    connect(player, &QMediaPlayer::durationChanged,
            this, &PlayerEngine::durationChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, [](auto &&x){
                binfo("media status changed to {} ", x);
            });
    // TODO: cc for qt6 building.
    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, [](auto && x){
                binfo("buffer status changed to {} ", x);
            });


    connect(player,QOverload<QMediaPlayer::Error, const QString &>::of(&QMediaPlayer::errorOccurred),
            [=](QMediaPlayer::Error error, const QString& msg){
                binfo("error {}", error);
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
    m_currentUrl = url;
    //TODO: qt6 building
    // player->setMedia(url);
    // player->setVolume(100);
    player->play();
    m_duration = player->duration();
}

void PlayerEngine::pause(){
    player->pause();
}

void PlayerEngine::resume() {
    if(!m_currentUrl.isEmpty() ){
        player->play();
    } else {
        binfo("resume, error, empty url");
    }
}

void PlayerEngine::stop(){
    m_currentUrl = QUrl{};
    player->stop();
}

void PlayerEngine::setPosition(int val)
{
    player->setPosition(val);
}

int PlayerEngine::duration() const
{
    return m_duration;
    return player->duration();
}


void PlayerEngine::setDuration(int val)
{
    binfo("duration changed to {}", val);
    m_duration = val;
}

void PlayerEngine::setVolume(int val)
{
    //TODO: qt6 building.
    // player->setVolume(val);

}

void PlayerEngine::seekforward()
{
    auto left = (player->duration() - player->position());
    if(left > m_jump_duration*1000){
        player->setPosition(player->position() + m_jump_duration*1000);
    } else {
    }
}


void PlayerEngine::seekbackward()
{
    auto cur = player->position();
    if(cur > m_jump_duration*1000)
        cur -= m_jump_duration*1000;
    else
        cur = 0;
    player->setPosition(cur);
}


void PlayerEngine::faster()
{
    m_speed += 0.1;
    this->player->setPlaybackRate(m_speed);
}


void PlayerEngine::slower()
{
    m_speed -= 0.1;
    this->player->setPlaybackRate(m_speed);
}


