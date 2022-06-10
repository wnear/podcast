
#include "player.h"
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include "player_engine.h"

namespace {
QString int2hms(int secs){
    int h,m,s;
    h = secs/3600;
    m = (secs%3600)/60;
    s = (secs%60);
    return QString("%1:%2:%3")
        .arg(h, 2,10,QChar('0'))
        .arg(m, 2,10,QChar('0'))
        .arg(s, 2,10,QChar('0'));
}
}

class PlayerPrivate {
public:
    QLabel *cover;
    QLabel *progress;
    QSlider *progressbar;

};
Player::Player(QWidget *parent):QFrame(parent)
{
    auto *lay = new QGridLayout(this);

    d = new PlayerPrivate;
    d->progress = new QLabel;
    d->progressbar = new QSlider(Qt::Horizontal, this);

    d->progress->setText(QString());
    d->progressbar->setRange(0, 100);
    lay->addWidget(d->progressbar);
    this->setLayout(lay);
    this->setFixedHeight(100);

    connect(d->progressbar, &QSlider::valueChanged, 
            PlayerEngine::instance(),&PlayerEngine::setProgress );
    connect( PlayerEngine::instance(),&PlayerEngine::progressChanged,
            d->progressbar, &QSlider::setValue);
    connect( PlayerEngine::instance(),&PlayerEngine::positionChanged,
           this, [this](int pos){
                int all = PlayerEngine::instance()->duration();
                d->progress->setText(QString("%1     %2")
                                     .arg(int2hms(pos))
                                     .arg(int2hms(all)));
            } );
}
