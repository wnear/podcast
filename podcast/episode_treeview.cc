#include "episode_treeview.h"
#include "episode_treemodel.h"
#include <QItemSelectionModel>
#include "episodedata.h"
#include "log.h"
#include "qevent.h"

#include <QMenu>
#include <QDebug>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QHeaderView>

#include <QGuiApplication>
#include <QClipboard>

class EpisodeTreeModel;

class EpisodeTreeViewPrivate {
  public:
    QAbstractItemModel *data{nullptr};
    EpisodeTreeSortFilterModel *sortmodel{nullptr};
    QItemSelectionModel *selectModel{nullptr};

    ~EpisodeTreeViewPrivate() { data->deleteLater(); }
};

EpisodeTreeView::EpisodeTreeView(QAbstractItemModel *model, QWidget *parent)
    : QTreeView(parent) {
    d = new EpisodeTreeViewPrivate;
    d->data = model;
    d->sortmodel = new EpisodeTreeSortFilterModel;
    d->sortmodel->setSourceModel(d->data);
    this->setModel(d->sortmodel);
    // TODO: default to SingleSelection??
    // d->selectModel = new QItemSelectionModel(d->data);
    // this->setSelectionModel(d->selectModel);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // QTreeView::setModel(d->sortmodel);

    // this->setRootIsDecorated(false);
    this->setSortingEnabled(true);

    // TODO: define a selectionModel when you have multi-view for one model,
    // to share the selection state.
    //

    // this->setEditTriggers(QAbstractItemView::DoubleClicked |
    // QAbstractItemView::SelectedClicked);

    // TODO: hover event,
    // this->installEventFilter(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    // TODO: mannual set visible columns.
    // this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(this->header(), &QWidget::customContextMenuRequested,
    //         [this](const QPoint &p) {
    //             auto menu = new QMenu;
    //             QAction *act;
    //             act = menu->addAction("@TODO: setup columns...");
    //             act->setCheckable(true);
    //             menu->exec(mapToGlobal(p));
    //         });

    connect(this, &QTreeView::clicked, this, [this](const QModelIndex &idx) {
        auto srcIdx = d->sortmodel->mapToSource(idx);
        auto *ep = static_cast<EpisodeData *>(srcIdx.internalPointer());
        emit requestDetail(ep);
    });

    auto *hidedetail = new QAction(this);
    hidedetail->setShortcuts(QKeySequence::Cancel);
    this->addAction(hidedetail);
    connect(hidedetail, &QAction::triggered, [this](){
        emit requestDetail(nullptr);
    });

    connect(this, &QWidget::customContextMenuRequested, this,
            &EpisodeTreeView::onCustomContextMenuRequested);
}

void EpisodeTreeView::onCustomContextMenuRequested(const QPoint &p) {
    auto menu = new QMenu;
    auto idx = this->indexAt(p);
    auto srcIdx = d->sortmodel->mapToSource(idx);

    if (!srcIdx.isValid()) return;

    // auto urlIdx = d->data->index(srcIdx.row(), TreeColumn::URL - 1);
    // auto url = d->data->data(urlIdx).toUrl();
    // emit requestPlay(url);

    auto *ep = static_cast<EpisodeData *>(srcIdx.internalPointer());
    menu->addAction("Detail", this, [this, ep]() { emit requestDetail(ep); });
    menu->addAction("Locate File");
    menu->addAction("Toggle Play");
    menu->addAction("Copy mediafile url to clipboard", [ep]() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ep->url.toString());
    });
    menu->addAction("Open With Default Player");
    menu->addSeparator();
    menu->addAction("Download or pause");
    // menu->addAction("Pause Downloading");
    menu->addAction("GoTo Download Management");
    menu->addAction("Delete");

    menu->exec(mapToGlobal(p));
}
// void EpisodeTreeView::mousePressEvent(QMouseEvent *event) {
//     auto index = this->indexAt(event->pos());
// }
void EpisodeTreeView::mouseDoubleClickEvent(QMouseEvent *event) {
    auto idx = this->indexAt(event->pos());
    auto srcIdx = d->sortmodel->mapToSource(idx);
    if (srcIdx.isValid()) {
        auto urlIdx = d->data->index(srcIdx.row(), TreeColumn::URL - 1);
        auto *ep = static_cast<EpisodeData *>(urlIdx.internalPointer());
        qDebug() << "double click to play:" << ep->title;
        // auto url = d->data->data(urlIdx).toUrl();
        emit requestPlay(ep);
    }
}
// bool EpisodeTreeView::eventFilter(QObject *obj, QEvent *evt) {
//     qDebug() << "eventfilter.";
//     if (evt->type() == QEvent::HoverEnter) {
//         binfo("treeview, hoverenter.");
//     } else if (evt->type() == QEvent::HoverLeave) {
//         binfo("treeview, hoverLeave.");
//     } else if (evt->type() == QEvent::HoverMove) {
//         binfo("treeview, hovermove.");
//     } else {
//     }
//     return QTreeView::eventFilter(obj, evt);
// }

EpisodeTreeView::~EpisodeTreeView() { delete d; }
