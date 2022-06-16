#include "episode_treewidget.h"
#include "episode_treeview.h"
#include "episode_treemodel.h"

#include <QVBoxLayout>
#include <QGridLayout>



class EpisodeTreeWidgetPrivate {
public:
    QWidget *base;
    EpisodeTreeView *view;
    QWidget *label;
};


EpisodeTreeWidget::EpisodeTreeWidget(QWidget *parent) 
:QFrame(parent)
{
    d = new EpisodeTreeWidgetPrivate;
    d->base = new QWidget(this);
    auto lay = new QVBoxLayout;
    d->view = new EpisodeTreeView(this);
    lay->addWidget(d->view);

    d->base->setLayout(lay);
}

void EpisodeTreeWidget::setPod(PodData *pod) 
{
    d->view->setPod(pod);
}
