#include "episode_treeview.h"
#include "episode_treemodel.h"
#include <QItemSelectionModel>
#include "log.h"

#include <QMenu>
#include <QDebug>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QHeaderView>

#include "player_engine.h"

class EpisodeTreeModel;

class EpisodeTreeViewPrivate{
public:
    EpisodeTreeModel *data{nullptr};
    EpisodeTreeSortFilterModel *sortmodel{nullptr};
    QItemSelectionModel *selectModel{nullptr};
    PlayerEngine *player{nullptr};

    ~EpisodeTreeViewPrivate(){
        data->deleteLater();
    }
};

EpisodeTreeView::EpisodeTreeView(QWidget *parent)
    :QTreeView(parent)
{
    d = new EpisodeTreeViewPrivate;
    d->player = PlayerEngine::instance();
    d->data = new EpisodeTreeModel;
    d->sortmodel = new EpisodeTreeSortFilterModel;
    d->sortmodel->setSourceModel(d->data);
    this->setModel(d->sortmodel);


    // this->setRootIsDecorated(false);
    this->setSortingEnabled(true);

    // TODO: define a selectionModel when you have multi-view for one model,
    // to share the selection state.
    //
    // d->selectModel = new QItemSelectionModel(d->data);
    // this->setSelectionModel(d->selectModel);

    //TODO: default to SingleSelection??
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // this->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // this->installEventFilter(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->header()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this->header(), &QWidget::customContextMenuRequested,[this](const QPoint &p){
        auto menu = new QMenu;
        QAction *act;
        act = menu->addAction("@TODO: setup columns...");
        act->setCheckable(true);
        menu->exec(mapToGlobal(p));
    });
    connect(this, &QWidget::customContextMenuRequested,
            this, &EpisodeTreeView::onCustomContextMenuRequested);
}
void EpisodeTreeView::setPod(PodcastChannel *pod)
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
    auto idx = this->indexAt(event->pos());
    qDebug()<<"treeview dbclick method cb...";
    auto srcIdx = d->sortmodel->mapToSource(idx);
    if(srcIdx.isValid()){
        auto urlIdx = d->data->index(srcIdx.row(), TreeColumn::URL -1);
        auto url = d->data->data(urlIdx).toUrl();
        qDebug()<<__PRETTY_FUNCTION__ << " start.";
        d->player->play(url);
        qDebug()<<__PRETTY_FUNCTION__ << " end.";
    }
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
