#include "mainwindow.h"

#include "episodedata.h"
#include "podcast.h"

#include "episode_detail_wgt.h"
#include <QSplitter>
#include <QTabWidget>

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QSettings>
#include <QCoreApplication>

#include <QTextEdit>
#include <QStackedWidget>
#include <QDebug>
#include <QFrame>
#include "playercontrolwidget.h"
#include <QVBoxLayout>
#include <QDockWidget>
#include <QPropertyAnimation>

#include "downloadmanagerwgt.h"
#include "qnamespace.h"

class Mainwindow::Private {
  public:
    QSplitter *base;
    QTabWidget *left;
    QFrame *right;
    QStackedWidget *rightdetail;
    PlayerControlWidget *rightPlayer;
    QDockWidget *dock;
    EpisodeDetailWidget *episode_detail;

    Podcast *localpod;
};

Mainwindow::Mainwindow(QWidget *parent) : QMainWindow(parent) {
    d = new Private;

    d->base = new QSplitter(this);
    d->rightPlayer = new PlayerControlWidget(this);

    d->left = new QTabWidget(this);
    d->localpod = new Podcast(this);
    d->left->addTab(d->localpod, "Podcasts");

    d->right = new QFrame(this);
    d->rightdetail = new QStackedWidget(this);
    auto *lay = new QVBoxLayout(d->right);
    lay->addWidget(d->rightdetail, 1);
    lay->addWidget(d->rightPlayer, 0);
    d->right->setLayout(lay);

    d->rightdetail->addWidget(d->localpod->detail());

    d->dock = new QDockWidget(this);
    d->dock->setAllowedAreas(Qt::RightDockWidgetArea);
    d->episode_detail = new EpisodeDetailWidget(this);
    d->dock->setWidget(d->episode_detail);
    d->dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    d->dock->hide();
    addDockWidget(Qt::RightDockWidgetArea, d->dock);

    connect(d->localpod, &Podcast::requestPlay, d->rightPlayer,
            &PlayerControlWidget::addMedia);
    connect(d->localpod, &Podcast::requestDetail, this, [this](EpisodeData *ep) {
        if(ep == nullptr){
            if(d->dock->isVisible())
                d->dock->hide();
        } else {
            d->episode_detail->setData(ep);
            if(d->dock->isHidden())
                showDetail();
        }
    });

    // d->left->addTab(d->subpod, "Subsonic Podcasts");
    // d->right->addWidget(d->subpod->detail());
    // d->left->addTab(d->subsong, "Subsonic songs");
    // d->right->addWidget(d->subsong->detail());

    d->base->addWidget(d->left);
    d->base->addWidget(d->right);

    this->setCentralWidget(d->base);

    QSettings settings(qApp->organizationName(), qApp->applicationName());
    d->base->restoreGeometry(settings.value("geometry/splitter").toByteArray());

    setupMenu();
    setupToolbar();

    connect(d->left, &QTabWidget::currentChanged, d->rightdetail,
            &QStackedWidget::setCurrentIndex);
}

//FIXME: animation not working.
void Mainwindow::showDetail() {
    d->dock->show();
    auto *animation = new QPropertyAnimation(d->dock, "geometry");
    qDebug()<<QString("dock size: (%1, %2)").arg(d->dock->width()).arg(d->dock->height());
    QPoint p1(width(), 0);
    QPoint p2(width() - d->dock->width(), 0);
    QRect rcStart, rcEnd;
    rcStart = QRect(p1, QSize(0, height()));
    rcEnd = QRect(p1, QSize(d->dock->width(), height()));
    animation->setDuration(1800);
    animation->setStartValue(rcStart);
    animation->setEndValue(rcEnd);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

Mainwindow::~Mainwindow() {
    qDebug() << __func__;
    delete d;
}

void Mainwindow::setupMenu() {
    auto *bar = this->menuBar();
    auto *_file = bar->addMenu("File");
    _file->addAction("import local podcast", [this]() { d->localpod->importdlg(); });
    bar->addMenu("Play");
    bar->addMenu("Help");
    bar->addMenu("About");
    bar->addMenu("...");
}
void Mainwindow::setupToolbar() {
    auto *bar = this->addToolBar("file");

    auto icon = QIcon::fromTheme("download");
    auto act = new QAction(icon, "hello");
    act->setToolTip("world");
    auto download = new QAction(icon, "download");
    connect(download, &QAction::triggered, [this]() {
        DownloadManagerWidget dlg(this);
        dlg.setModal(true);
        qDebug() << "before exec";
        dlg.exec();
        qDebug() << "after exec";
    });

    bar->addAction(act);
    bar->addAction(download);

    bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
}

// FIXME: geometry save/restore didnt work as expected.
void Mainwindow::closeEvent(QCloseEvent *event) {
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    settings.setValue("geometry/splitter", d->base->saveGeometry());
    qDebug() << "close event before";
    QMainWindow::closeEvent(event);
    qDebug() << "close event after";
}
