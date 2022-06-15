#include "episode_treemodel.h"
#include "episodedata.h"
#include "poddata.h"

#include <algorithm>

class EpisodeTreeModelPrivate {
public:
    QList<PodData *> pods{};
    QMap<int, PodData*> pod_idx;
    QMap<TreeColumn, QString> availproperty;
    void setdata() {
        pod_idx.clear();
        int idx = 0;
    }
};

EpisodeTreeModel::EpisodeTreeModel(QObject *parent) 
: QAbstractItemModel(parent)
{
    d = new EpisodeTreeModelPrivate;
    d->availproperty = {
        {TreeColumn::TITLE, "title"},
        {TreeColumn::SIZE, "file size"},
    };
}

void EpisodeTreeModel::setPod(PodData *pod) {
    d->pods.clear();
    d->pods.push_back(pod);
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
    return d->pods.length() == 0 
            ? 0
            : d->pods[0]->episodes.count();
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
    auto realIndex = d->availproperty.keys().at(index.row());

    switch(realIndex){
        case TreeColumn::TITLE: 
            return ep->title;
            break;
        case TreeColumn::UPDATETIME: 
            break;
        case TreeColumn::SIZE: 
            return ep->filesize;
            break;
        case TreeColumn::DURATION: 
            return ep->duration;
            break;
        case TreeColumn::LASTVIEW: 
            break;
        default:
            break;
    }
    return QVariant();
}
QVariant EpisodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
