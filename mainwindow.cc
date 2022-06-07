#include "mainwindow.h"

#include "podcast.h"
#include "subpodcast.h"
#include "subsong.h"

#include <QSplitter>
#include <QTabWidget>

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QTextEdit>
#include <QStackedWidget>

class Mainwindow::Private
    {
    public:
    QSplitter *base;
    QTabWidget *left; 
    QStackedWidget * right; 

        Podcast *localpod;
        SubPodcast *subpod;
        SubSong *subsong;
    };

Mainwindow::Mainwindow(QWidget *parent):
    QMainWindow(parent)
{
    d = new Private;

    d->base = new QSplitter(this);
    d->left = new QTabWidget(this);
    d->right = new QStackedWidget(this);

    d->localpod = new Podcast(this);
    d->subpod = new SubPodcast(this);
    d->subsong = new SubSong(this);

    d->left->addTab(d->localpod, "Podcasts");
    d->right->addWidget(d->localpod->detail());

    // d->left->addTab(d->subpod, "Subsonic Podcasts");
    // d->right->addWidget(d->subpod->detail());
    // d->left->addTab(d->subsong, "Subsonic songs");
    // d->right->addWidget(d->subsong->detail());

    connect(d->left, &QTabWidget::currentChanged, d->right, &QStackedWidget::setCurrentIndex);
    d->base->addWidget(d->left);
    d->base->addWidget(d->right);

    this->setCentralWidget(d->base);
    setupMenu();
}
Mainwindow::~Mainwindow()
{
    delete d;
}

void Mainwindow::setupMenu()
{
    auto *bar = this->menuBar();
    auto *_file = bar->addMenu("File");
    _file->addAction("import local podcast", [this](){
                         d->localpod->importdlg();
                     });
    bar->addMenu("Play");
    bar->addMenu("Help");
    bar->addMenu("About");
    bar->addMenu("...");

}
