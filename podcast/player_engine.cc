#include "player_engine.h"
#include "log.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <memory>

static QMutex creatMutex;

std::shared_ptr<PlayerEngine > PlayerEngine::ins = nullptr;
std::shared_ptr<PlayerEngine> PlayerEngine::instance() {
    if (ins == nullptr) {
        QMutexLocker lock(&creatMutex);
        ins = std::make_shared<PlayerEngine>(nullptr);
    }
    return ins;
}

PlayerEngine::PlayerEngine(QObject *parent) : QObject(parent) {
    m_player = new QMediaPlayer(this);
    m_player_audio_ctrl = new QAudioOutput(this);
    m_player->setAudioOutput(m_player_audio_ctrl);
    assert(m_player != nullptr);
    connect(m_player, &QMediaPlayer::positionChanged, this,
            &PlayerEngine::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayerEngine::setDuration);
    connect(m_player, &QMediaPlayer::durationChanged, this,
            &PlayerEngine::durationChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this,
            [](auto &&x) { binfo("media status changed to {} ", x); });
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this,
            [](auto &&x) { binfo("buffer status changed to {} ", x); });

    connect(
        m_player,
        QOverload<QMediaPlayer::Error, const QString &>::of(&QMediaPlayer::errorOccurred),
        [=](QMediaPlayer::Error error, const QString &msg) { binfo("error {}", error); });
}

void PlayerEngine::play(const QString &url) {
    qDebug() << "local file";
    this->play(QUrl::fromLocalFile(url));
}

void PlayerEngine::play(QUrl url) {
    qDebug() << "trying to play: " << url.toString();
    m_player->stop();
    m_currentUrl = url;
    m_player->setSource(url);
    m_player_audio_ctrl->setVolume(100);
    // TODO: wait in local Eventloop, to get GUI smooth.
    m_player->play();
    m_duration = m_player->duration();

    qDebug() << __PRETTY_FUNCTION__ << " call end.";
}

void PlayerEngine::pause() { m_player->pause(); }

void PlayerEngine::resume() {
    if (!m_currentUrl.isEmpty()) {
        m_player->play();
    } else {
        binfo("resume, error, empty url");
    }
}

void PlayerEngine::stop() {
    m_currentUrl = QUrl{};
    m_player->stop();
}

void PlayerEngine::setPosition(int val) { m_player->setPosition(val); }

int PlayerEngine::duration() const {
    return m_duration;
    return m_player->duration();
}

void PlayerEngine::setDuration(int val) {
    binfo("duration changed to {}", val);
    m_duration = val;
}

void PlayerEngine::setVolume(int val) { m_player_audio_ctrl->setVolume(val); }

void PlayerEngine::seekforward() {
    auto left = (m_player->duration() - m_player->position());
    if (left > m_jump_duration * 1000) {
        m_player->setPosition(m_player->position() + m_jump_duration * 1000);
    } else {
    }
}

void PlayerEngine::seekbackward() {
    auto cur = m_player->position();
    if (cur > m_jump_duration * 1000)
        cur -= m_jump_duration * 1000;
    else
        cur = 0;
    m_player->setPosition(cur);
}

void PlayerEngine::faster() {
    m_speed += 0.1;
    this->m_player->setPlaybackRate(m_speed);
}

void PlayerEngine::slower() {
    m_speed -= 0.1;
    this->m_player->setPlaybackRate(m_speed);
}
