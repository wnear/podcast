#include "mainwindow.h"

#include "podcast.h"

#include <QSplitter>
#include <QTabWidget>

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>

#include <QTextEdit>
#include <QStackedWidget>
#include <QDebug>
#include <QFrame>
#include "player.h"
#include <QVBoxLayout>

#include "downloadmanagerwgt.h"

class Mainwindow::Private {
  public:
    QSplitter *base;
    QTabWidget *left;
    QFrame *right;
    QStackedWidget *rightdetail;
    Player *rightPlayer;

    Podcast *localpod;
};

Mainwindow::Mainwindow(QWidget *parent) : QMainWindow(parent) {
    d = new Private;

    d->base = new QSplitter(this);
    d->left = new QTabWidget(this);

    d->right = new QFrame(this);
    d->rightdetail = new QStackedWidget(this);
    d->rightPlayer = new Player(this);
    auto *lay = new QVBoxLayout(d->right);
    lay->addWidget(d->rightdetail, 1);
    lay->addWidget(d->rightPlayer, 0);
    d->right->setLayout(lay);

    d->localpod = new Podcast(this);

    d->left->addTab(d->localpod, "Podcasts");
    d->rightdetail->addWidget(d->localpod->detail());

    // d->left->addTab(d->subpod, "Subsonic Podcasts");
    // d->right->addWidget(d->subpod->detail());
    // d->left->addTab(d->subsong, "Subsonic songs");
    // d->right->addWidget(d->subsong->detail());

    connect(d->left, &QTabWidget::currentChanged, d->rightdetail,
            &QStackedWidget::setCurrentIndex);
    d->base->addWidget(d->left);
    d->base->addWidget(d->right);

    this->setCentralWidget(d->base);
    setupMenu();
    setupToolbar();
    this->resize(2000, 1600);
}
Mainwindow::~Mainwindow() { delete d; }

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
