#include "playercontrolwidget.h"
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include "sqlmanager.h"
#include "player_engine.h"
#include "podcastchannel.h"
#include "episodedata.h"
#include "utils.h"
#include "log.h"
#include <QOpenGLWidget>
#include <QToolButton>
#include <QAction>
#include <memory>
#include <iostream>

namespace {}


using namespace util;
using namespace std;

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

    QLabel *pos;
    QSlider *progressbar;
    std::shared_ptr<PlayerEngine> engine;

    EpisodeData *cur_episode{nullptr};
    ~PlayerPrivate(){
        cout << "~PlayerPrivate() called."<<endl;
    }
};
PlayerControlWidget::~PlayerControlWidget(){
    save_playposition();
    cout << "~PlayerControlWidget() called."<<endl;
};
PlayerControlWidget::PlayerControlWidget(QWidget *parent) : QFrame(parent) {
    d = new PlayerPrivate;
    assert(PlayerEngine::is_inited() == false);
    d->engine = PlayerEngine::instance();
    assert(PlayerEngine::is_inited() == true);

    auto *lay = new QHBoxLayout(this);

    d->cover = new QOpenGLWidget(this);
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
        connect(&d->play, &QToolButton::clicked, d->engine.get(), &PlayerEngine::resume);
        connect(&d->pause, &QToolButton::clicked, d->engine.get(), &PlayerEngine::pause);
        connect(&d->stop, &QToolButton::clicked, d->engine.get(), &PlayerEngine::stop);
        connect(&d->jumpback, &QToolButton::clicked, d->engine.get(),
                &PlayerEngine::seekforward);
        connect(&d->jumpforward, &QToolButton::clicked, d->engine.get(),
                &PlayerEngine::seekbackward);
        connect(&d->slower, &QToolButton::clicked, d->engine.get(), &PlayerEngine::slower);
        connect(&d->faster, &QToolButton::clicked, d->engine.get(), &PlayerEngine::faster);
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
        d->pos = new QLabel;
        // d->pos->setText(QString("Position: "));
        d->pos->setText( QString("%1     %2").arg(int2hms(0)).arg(int2hms(0)));

        d->progressbar = new QSlider(Qt::Horizontal, this);
        d->progressbar->setRange(0, 100);
        d->progressbar->setTracking(false);
    }
    right_bottom->addWidget(d->pos);
    right_bottom->addWidget(d->progressbar);

    this->setLayout(lay);
    // this->setFixedHeight(100);

    connect(d->progressbar, &QSlider::sliderMoved, this, [this](int val) {
        { d->engine->setPosition(val * 1000); }
    });
    connect(d->engine.get(), &PlayerEngine::positionChanged, this, [this](int val) {
        if (!d->progressbar->isSliderDown()) d->progressbar->setValue(val / 1000);
    });
    connect(d->engine.get(), &PlayerEngine::durationChanged, this, [this](int val) {
        d->progressbar->setMaximum(val / 1000);
    });
    connect(d->engine.get(), &PlayerEngine::positionChanged, this, [this](int pos) {
        int all = PlayerEngine::instance()->duration();
        d->pos->setText(
            QString("%1     %2").arg(int2hms(pos / 1000)).arg(int2hms(all / 1000)));
    });
}
void PlayerControlWidget::Pause() { d->engine->pause(); }

void PlayerControlWidget::Play() { d->engine->resume(); }

void PlayerControlWidget::Stop() { d->engine->stop(); }

void PlayerControlWidget::setVolume(int x) { d->engine->setVolume(x); }
void PlayerControlWidget::addMedia(EpisodeData *ep) {
    save_playposition();
    d->cur_episode = ep;
    //TODO: option to play from last time or from media-start.
    d->engine->play(ep->url, ep->play_position);
}

void PlayerControlWidget::save_playposition() {
    if(d->cur_episode == nullptr){
        return;
    }
    d->cur_episode->play_position = d->engine->position();
    SQLManager::instance()->updateEpisodePlayposition(d->cur_episode);
}

