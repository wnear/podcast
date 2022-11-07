#ifndef _PLAYER_ENGINE_H
#define _PLAYER_ENGINE_H

#include <QObject>
#include <QUrl>

class QMediaPlayer;

class PlayerEngine : public QObject
{
    Q_OBJECT

    public:
        static PlayerEngine *instance(QObject *parent = nullptr);
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
        int duration()const;
    public slots:
        void setPosition(int);
        void setDuration(int);
    signals:
        void positionChanged(int);
        void durationChanged(int);
    private:
        PlayerEngine(QObject* parent = nullptr);
        QMediaPlayer *player;
        static PlayerEngine *ins;
        int m_duration;
        QUrl m_currentUrl;
        float m_speed = 1.0;
        int m_jump_duration = 30; // in seconds.
};

#endif

