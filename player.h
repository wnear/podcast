#ifndef __PLAYER_H
#define __PLAYER_H

#include <QString>

class Player
{
    public:
        Player();
        void Pause ();
        void Play();
        void Stop();
        void setVolume(int x);

        ~Player();

    private:
        QString m_currentMedia;
        int m_position;
        int m_volume;

};


#endif // !__PLAYER_H
