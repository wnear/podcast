#include "episode_treewidget.h"
#include "episode_treeview.h"
#include "episode_treemodel.h"

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

    QToolButton update;
    QComboBox sortBy;
    QToolButton filter;
    QToolButton fetchmore;

    QLabel status;
};


EpisodeTreeWidget::EpisodeTreeWidget(QWidget *parent) 
:QFrame(parent)
{
    d = new EpisodeTreeWidgetPrivate;

    auto lay = new QVBoxLayout;

    auto head = new QHBoxLayout;
    head->addWidget(&(d->update));
    d->update.setIcon(QIcon::fromTheme("update"));

    head->addWidget(&(d->filter));
    auto menu = new QMenu;
    menu->addAction("hello");
    d->filter.setMenu(menu);
    d->filter.setPopupMode(QToolButton::InstantPopup);
    d->filter.setArrowType(Qt::NoArrow);


    head->addStretch(1);
    head->addWidget(&(d->fetchmore));

    d->view = new EpisodeTreeView(this);

    auto bottom = new QHBoxLayout;
    bottom->addWidget(&(d->status));
    bottom->addStretch(1);

    lay->addLayout(head);
    lay->addWidget(d->view);
    lay->addLayout(bottom);

    this->setLayout(lay);
}

void EpisodeTreeWidget::setPod(PodData *pod) 
{
    d->view->setPod(pod);
}
EpisodeTreeWidget::~EpisodeTreeWidget() { delete d;}
