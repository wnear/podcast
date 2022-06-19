#include "episode_treeview.h"
#include "episode_treemodel.h"
#include "log.h"

#include <QMenu>
#include <QMouseEvent>
#include <QHoverEvent>

class EpisodeTreeModel;

class EpisodeTreeViewPrivate{
public:
    EpisodeTreeModel *data{nullptr};
    ~EpisodeTreeViewPrivate(){
        data->deleteLater();
    }
};

EpisodeTreeView::EpisodeTreeView(QWidget *parent) 
    :QTreeView(parent)
{
    d = new EpisodeTreeViewPrivate;
    d->data = new EpisodeTreeModel;
    this->setModel(d->data);

    this->installEventFilter(this);
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
bool EpisodeTreeView::eventFilter(QObject *obj, QEvent *evt) {
    
    if(evt->type() == QEvent::HoverEnter){
        binfo("treeview, hoverenter.");
    } else if(evt->type() == QEvent::HoverLeave){

        binfo("treeview, hoverLeave.");
    } else if(evt->type() == QEvent::HoverMove){

        binfo("treeview, hovermove.");
    } else {

    }
    return  QTreeView::eventFilter(obj, evt);
}
EpisodeTreeView::~EpisodeTreeView(){ delete d;}
