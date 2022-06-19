#ifndef __PLAYER_H
#define __PLAYER_H

#include <QString>
#include <QFrame>

class Player : public QFrame
{
    public:
        Player(QWidget *parent);
        void Pause();
        void Play();
        void Stop();

        void faster();
        void slower();
        void resetSpeed();
        void setspeed();

        void jumpforward();
        void jumpbackward();
        void setJumprange(int val){
            m_jumprange = val;
        }

        void setVolume(int x);

        ~Player(){};

    private:
        class PlayerPrivate *d;
        QString m_currentMedia;
        int m_position;
        int m_volume;

        int m_jumprange{30}; // 30s.
        float m_currentspeed{1.0};

};


#endif // !__PLAYER_H
