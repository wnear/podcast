
#include "player.h"
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include "player_engine.h"
#include "utils.h"
#include "log.h"

namespace {
}

class PlayerPrivate {
public:
    QLabel *cover;
    QLabel *info;
    QLabel *pos;
    QSlider *progressbar;

};
Player::Player(QWidget *parent):QFrame(parent)
{
    auto *lay = new QVBoxLayout(this);

    d = new PlayerPrivate;

    d->info = new QLabel;
    d->info->setText(QString("Duration: "));
    d->pos = new QLabel;
    d->pos->setText(QString("Position: "));

    d->progressbar = new QSlider(Qt::Horizontal, this);
    d->progressbar->setRange(0, 100);
    d->progressbar->setTracking(false);

    lay->addWidget(d->info);
    lay->addWidget(d->pos);

    lay->addWidget(d->progressbar);
    this->setLayout(lay);
    //this->setFixedHeight(100);

    connect(d->progressbar, &QSlider::sliderMoved,
            PlayerEngine::instance(),[this, player = PlayerEngine::instance()](int val){
                {
                    player->setPosition(val * 1000);
                }
            });
    connect( PlayerEngine::instance(),&PlayerEngine::positionChanged,
            this, [this](int val){
                if(!d->progressbar->isSliderDown())
                d->progressbar->setValue(val/1000);
            });
    connect( PlayerEngine::instance(),&PlayerEngine::durationChanged,
            this, [this](int val){
                d->info->setText(QString("Duratin:(%1) %2").arg(val/1000).arg(int2hms(val/1000)));
                d->progressbar->setMaximum(val/1000);
            });
    connect( PlayerEngine::instance(),&PlayerEngine::positionChanged,
           this, [this](int pos){
                int all = PlayerEngine::instance()->duration();
                d->pos->setText(QString("%1     %2")
                                     .arg(int2hms(pos/1000))
                                     .arg(int2hms(all/1000)));
            } );
}
