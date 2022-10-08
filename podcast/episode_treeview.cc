#include "episode_treeview.h"
#include "episode_treemodel.h"
#include <QItemSelectionModel>
#include "log.h"

#include <QMenu>
#include <QMouseEvent>
#include <QHoverEvent>

class EpisodeTreeModel;

class EpisodeTreeViewPrivate{
public:
    EpisodeTreeModel *data{nullptr};
    EpisodeTreeSortFilterModel *sortmodel;
    QItemSelectionModel *selectModel;

    ~EpisodeTreeViewPrivate(){
        data->deleteLater();
    }
};

EpisodeTreeView::EpisodeTreeView(QWidget *parent)
    :QTreeView(parent)
{
    d = new EpisodeTreeViewPrivate;
    d->data = new EpisodeTreeModel;
    d->sortmodel = new EpisodeTreeSortFilterModel;
    d->selectModel = new QItemSelectionModel(d->data);

    d->sortmodel->setSourceModel(d->data);

    this->setRootIsDecorated(false);
    this->setModel(d->sortmodel);
    this->setSortingEnabled(true);
    this->setSelectionModel(d->selectModel);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    this->installEventFilter(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &EpisodeTreeView::onCustomContextMenuRequested);
        //double click.
    connect(this,&QAbstractItemView::doubleClicked, [this](const QModelIndex &index){
                auto c = index.siblingAtColumn(TreeColumn::LOCATION);
                auto location = this->model()->data(c).toString();
            } );
}
void EpisodeTreeView::setPod(PodData *pod)
{
    d->data->setPod(pod);
    this->reset();
}

void EpisodeTreeView::onCustomContextMenuRequested(const QPoint &p)
{
    auto menu = new QMenu;
    menu->addAction("Detail");
    menu->addAction("Locate File");
    menu->addAction("Toggle Play");
    menu->addAction("Open With Default Player");
    menu->addSeparator();
    menu->addAction("Download or pause");
    //menu->addAction("Pause Downloading");
    menu->addAction("GoTo Download Management");
    menu->addAction("Delete");

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
