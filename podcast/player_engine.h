#ifndef _PLAYER_ENGINE_H
#define _PLAYER_ENGINE_H

#include <QObject>
#include <QUrl>

class QMediaPlayer;
class QAudioOutput;

class PlayerEngine : public QObject , public std::enable_shared_from_this<PlayerEngine>{
    Q_OBJECT

  public:
    static std::shared_ptr<PlayerEngine> instance();
    static bool is_inited() { return ins != nullptr; }
    PlayerEngine(QObject *parent = nullptr);
    ~PlayerEngine() = default;
    void play(const QString &url);
    void play(QUrl);
    void setVolume(int);
    void resume();
    void pause();
    void seekforward();
    void seekbackward();
    void faster();
    void slower();
    void stop();
    int duration() const;
  public slots:
    void setPosition(int);
    void setDuration(int);
  signals:
    void positionChanged(int);
    void durationChanged(int);

  private:
    QMediaPlayer *m_player;
    QAudioOutput *m_player_audio_ctrl;
    static std::shared_ptr<PlayerEngine> ins;
    int m_duration;
    QUrl m_currentUrl;
    float m_speed = 1.0;
    int m_jump_duration = 30;  // in seconds.
};

#endif
