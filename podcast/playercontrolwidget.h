#ifndef __PLAYERCONTROLWIDGET_H
#define __PLAYERCONTROLWIDGET_H

#include <QString>
#include <QFrame>
#include "qurl.h"

class EpisodeData;

class PlayerControlWidget : public QFrame {
  public:
    PlayerControlWidget(QWidget *parent);
    void Pause();
    void Play();
    void Stop();

    void faster();
    void slower();
    void resetSpeed();
    void setspeed();
    void addMedia(EpisodeData *ep);

    void jumpforward();
    void jumpbackward();
    void setJumprange(int val) { m_jumprange = val; }

    void setVolume(int x);

    ~PlayerControlWidget();

  private:
    class PlayerPrivate *d;
    QString m_currentMedia;
    int m_position;
    int m_volume;

    int m_jumprange{30};  // 30s.
    float m_currentspeed{1.0};
};

#endif  // !__PLAYER_H
