#include "episode_treemodel.h"
#include "episodedata.h"
#include "poddata.h"
#include "log.h"
#include "utils.h"

#include <algorithm>

class EpisodeTreeModelPrivate {
public:
    int maxcolumn = 30;
    QList<PodData *> pods{};
    void reset() {
        pods.clear();
        pod_idx.clear();
        maxcolumn = 30;
    }
    QMap<int, PodData*> pod_idx;
    QMap<TreeColumn, QString> availproperty;
    QMap<TreeColumn, QString> unseen;
};

EpisodeTreeModel::EpisodeTreeModel(QObject *parent) 
: QAbstractItemModel(parent)
{
    d = new EpisodeTreeModelPrivate;
    d->availproperty = {
        {TreeColumn::TITLE, "title"},
        {TreeColumn::SIZE, "file size"},
        {TreeColumn::DURATION, "duration"},
    };
    d->unseen = {
    };
    for(auto i: d->availproperty.keys())
    {
        binfo("d.keys: {} ", static_cast<int>(i));
    }
}

void EpisodeTreeModel::setPod(PodData *pod) {
    beginResetModel();
    d->reset();
    d->pods.push_back(pod);
    endResetModel();
}

QModelIndex EpisodeTreeModel::index(int row, int column, const QModelIndex &parent) const 
{
    return (row > 0 && row < this->rowCount())
            ? createIndex(row, column, nullptr)
            : QModelIndex();
}

QModelIndex EpisodeTreeModel::parent(const QModelIndex &child) const 
{

    return QModelIndex();
}

//TODO: fetchMore api.
//TODO: use std::accumulate from c++20
int EpisodeTreeModel::rowCount(const QModelIndex &parent) const 
{
    if(d->pods.length() == 0) 
        return 0;
    return std::min(d->maxcolumn, d->pods[0]->episodes.count());
}

int EpisodeTreeModel::columnCount(const QModelIndex &parent) const 
{
    return d->availproperty.count();
}

QVariant EpisodeTreeModel::data(const QModelIndex &index, int role) const 
{
    if(! index.isValid())
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();

    auto && ep = d->pods[0]->episodes[index.row()];
    auto realIndex = d->availproperty.keys().at(index.column());
    switch(realIndex){
        case TreeColumn::TITLE: 
            return ep->title;
        case TreeColumn::UPDATETIME: 
            break;
        case TreeColumn::SIZE: 
            return size_human(ep->filesize);
        case TreeColumn::DURATION: 
            return int2hms(ep->duration);
        case TreeColumn::LASTVIEW: 
            break;
        default:
            break;
    }
    return QVariant();
}

QVariant EpisodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        auto key = d->availproperty.keys().at(section);
        return d->availproperty[key];
    }
    return QVariant();
}

bool EpisodeTreeModel::hasChildren(const QModelIndex &parent) const 
{
    return !parent.isValid();
}
