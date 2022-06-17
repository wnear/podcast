#include "episode_treeview.h"
#include "episode_treemodel.h"

#include <QMenu>
#include <QMouseEvent>

class EpisodeTreeModel;

class EpisodeTreeViewPrivate{
public:
    EpisodeTreeModel *data{nullptr};
};

EpisodeTreeView::EpisodeTreeView(QWidget *parent) 
    :QTreeView(parent)
{
    d = new EpisodeTreeViewPrivate;
    d->data = new EpisodeTreeModel;
    this->setModel(d->data);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, 
            this, &EpisodeTreeView::onCustomContextMenuRequested);
}
void EpisodeTreeView::setPod(PodData *pod) 
{
    d->data->setPod(pod);
    this->reset();
}

void EpisodeTreeView::onCustomContextMenuRequested(const QPoint &p) 
{
    auto menu = new QMenu;
    menu->addAction("Hello");

    menu->exec(mapToGlobal(p));
}
void EpisodeTreeView::mousePressEvent(QMouseEvent *event)
{
    auto index = this->indexAt(event->pos());
}
void EpisodeTreeView::mouseDoubleClickEvent(QMouseEvent *event) 
{
    auto index = this->indexAt(event->pos());
}
