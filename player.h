#ifndef __PLAYER_H
#define __PLAYER_H

#include <QString>
#include <QFrame>

class Player : public QFrame
{
    public:
        Player(QWidget *parent);
        void Pause ();
        void Play();
        void Stop();
        void setVolume(int x);

        ~Player(){};

    private:
        class PlayerPrivate *d;
        QString m_currentMedia;
        int m_position;
        int m_volume;

};


#endif // !__PLAYER_H
