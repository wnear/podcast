#ifndef _PLAYER_ENGINE_H
#define _PLAYER_ENGINE_H

#include <QObject>
#include <QUrl>

class QMediaPlayer;

class PlayerEngine : public QObject
{
    Q_OBJECT
    public:
        PlayerEngine(QObject* parent = nullptr);
        ~PlayerEngine() = default;
        void play(const QString &url);
        void play(QUrl );
        void pause();
        void stop();
    private:
        QMediaPlayer *player;
};

#endif


