#include "episode_treeview.h"
#include "episode_treemodel.h"

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
}
void EpisodeTreeView::setPod(PodData *pod) 
{
    d->data->setPod(pod);
    this->reset();
}
