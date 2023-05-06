#include "player.h"
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include "player_engine.h"
#include "utils.h"
#include "log.h"
#include <QOpenGLWidget>
#include <QToolButton>
#include <QAction>

namespace {}


using namespace util;

class PlayerPrivate {
  public:
    QOpenGLWidget *cover;
    QToolButton play;
    QToolButton pause;
    QToolButton stop;
    QToolButton jumpforward;
    QToolButton jumpback;
    QToolButton faster;
    QToolButton slower;
    QList<QToolButton *> btns;

    QLabel *info;
    QLabel *pos;
    QSlider *progressbar;
    PlayerEngine *engine;
};
Player::Player(QWidget *parent) : QFrame(parent) {
    d = new PlayerPrivate;
    d->engine = PlayerEngine::instance();

    auto *lay = new QHBoxLayout(this);

    d->cover = new QOpenGLWidget;
    auto right = new QVBoxLayout;
    lay->addWidget(d->cover);
    lay->addLayout(right);

    auto right_up = new QHBoxLayout;
    auto right_bottom = new QHBoxLayout;
    right->addLayout(right_up);
    right->addLayout(right_bottom);

    {
        d->play.setIcon(QIcon::fromTheme("media-playback-start"));
        d->pause.setIcon(QIcon::fromTheme("media-playback-paused"));
        d->stop.setIcon(QIcon::fromTheme("media-playback-stop"));
        d->jumpforward.setIcon(QIcon::fromTheme("media-skip-forward"));
        d->jumpback.setIcon(QIcon::fromTheme("media-skip-backward"));
        d->faster.setIcon(QIcon::fromTheme("media-seek-forward"));
        d->slower.setIcon(QIcon::fromTheme("media-seek-backward"));
        connect(&d->play, &QToolButton::clicked, d->engine, &PlayerEngine::resume);
        connect(&d->pause, &QToolButton::clicked, d->engine, &PlayerEngine::pause);
        connect(&d->stop, &QToolButton::clicked, d->engine, &PlayerEngine::stop);
        connect(&d->jumpback, &QToolButton::clicked, d->engine,
                &PlayerEngine::seekforward);
        connect(&d->jumpforward, &QToolButton::clicked, d->engine,
                &PlayerEngine::seekbackward);
        connect(&d->slower, &QToolButton::clicked, d->engine, &PlayerEngine::slower);
        connect(&d->faster, &QToolButton::clicked, d->engine, &PlayerEngine::faster);
    }

    right_up->addWidget(&d->play);
    right_up->addWidget(&d->pause);
    right_up->addWidget(&d->stop);
    right_up->addWidget(&d->jumpback);
    right_up->addWidget(&d->jumpforward);
    right_up->addWidget(&d->slower);
    right_up->addWidget(&d->faster);
    right_up->addStretch(1);

    {
        d->info = new QLabel;
        d->info->setText(QString("Duration: "));
        d->pos = new QLabel;
        d->pos->setText(QString("Position: "));

        d->progressbar = new QSlider(Qt::Horizontal, this);
        d->progressbar->setRange(0, 100);
        d->progressbar->setTracking(false);
    }
    right_bottom->addWidget(d->info);
    right_bottom->addWidget(d->pos);
    right_bottom->addWidget(d->pos);
    right_bottom->addWidget(d->progressbar);

    this->setLayout(lay);
    // this->setFixedHeight(100);

    connect(d->progressbar, &QSlider::sliderMoved, this, [this](int val) {
        { d->engine->setPosition(val * 1000); }
    });
    connect(d->engine, &PlayerEngine::positionChanged, this, [this](int val) {
        if (!d->progressbar->isSliderDown()) d->progressbar->setValue(val / 1000);
    });
    connect(d->engine, &PlayerEngine::durationChanged, this, [this](int val) {
        d->info->setText(
            QString("Duratin:(%1) %2").arg(val / 1000).arg(int2hms(val / 1000)));
        d->progressbar->setMaximum(val / 1000);
    });
    connect(d->engine, &PlayerEngine::positionChanged, this, [this](int pos) {
        int all = PlayerEngine::instance()->duration();
        d->pos->setText(
            QString("%1     %2").arg(int2hms(pos / 1000)).arg(int2hms(all / 1000)));
    });
}
void Player::Pause() { d->engine->pause(); }

void Player::Play() { d->engine->resume(); }

void Player::Stop() { d->engine->stop(); }

void Player::setVolume(int x) { d->engine->setVolume(x); }
