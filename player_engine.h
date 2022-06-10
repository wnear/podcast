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
        void pause();
        void stop();
        int duration()const;
        void setPosition(int);
    public slots:
        void setProgress(int);
    signals:
        void positionChanged(int);
        void progressChanged(int);
    private:
        PlayerEngine(QObject* parent = nullptr);
        QMediaPlayer *player;
        static PlayerEngine *ins;
};

#endif


