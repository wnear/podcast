#include "episode_treewidget.h"
#include "episode_treeview.h"
#include "episode_treemodel.h"
#include "podcastchannel.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QMenu>

class EpisodeTreeWidgetPrivate {
  public:
    EpisodeTreeView *view;
    EpisodeTreeModel *model;

    QToolButton update;
    QComboBox sortBy;
    QToolButton filter;
    QToolButton fetchmore;
    PodcastChannel *data;

    QLabel status;
};

EpisodeTreeWidget::EpisodeTreeWidget(QWidget *parent) : QFrame(parent) {
    d = new EpisodeTreeWidgetPrivate;

    auto lay = new QVBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);

    auto head = new QHBoxLayout;
    head->addWidget(&(d->update));
    d->update.setIcon(QIcon::fromTheme("update"));

    head->addWidget(&(d->filter));
    auto menu = new QMenu;
    auto act = menu->addAction("DownloadedOnly");
    act->setCheckable(true);
    d->filter.setMenu(menu);
    d->filter.setPopupMode(QToolButton::InstantPopup);
    d->filter.setArrowType(Qt::NoArrow);

    head->addStretch(1);
    head->addWidget(&(d->fetchmore));

    d->model = new EpisodeTreeModel;
    d->view = new EpisodeTreeView(d->model, this);

    auto bottom = new QHBoxLayout;
    bottom->addWidget(&(d->status));
    bottom->addStretch(1);

    lay->addLayout(head);
    lay->addWidget(d->view);
    lay->addLayout(bottom);

    this->setLayout(lay);

    connect(d->view, &EpisodeTreeView::requestPlay, this, &EpisodeTreeWidget::requestPlay);
    connect(d->view, &EpisodeTreeView::requestPlay, this, &EpisodeTreeWidget::requestPlay);
}

void EpisodeTreeWidget::setPod(PodcastChannel *pod) {
    d->model->setupModelData(pod);
    d->view->reset();
}
EpisodeTreeWidget::~EpisodeTreeWidget() { delete d; }
