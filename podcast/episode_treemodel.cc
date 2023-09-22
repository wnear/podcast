#include "episode_treemodel.h"
#include "episodedata.h"
#include "podcastchannel.h"
#include "log.h"
#include "utils.h"

#include <QIcon>
#include <QDebug>
#include <algorithm>

using namespace util;
class EpisodeTreeModelPrivate {
  public:
    int maxRow = 30;
    PodcastChannel *pod{nullptr};
    void reset() { pod = nullptr; }
    QMap<TreeColumn, QString> availproperty;
    QMap<TreeColumn, QString> allproperty;
};

EpisodeTreeModel::EpisodeTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    d = new EpisodeTreeModelPrivate;
    d->availproperty = {{TreeColumn::TITLE, tr("title")},
                        {TreeColumn::SIZE, tr("file size")},
                        {TreeColumn::DURATION, tr("duration")},
                        {TreeColumn::URL, tr("URL")},
                        {TreeColumn::DATETIME_UPDATED, tr("update")},
                        {TreeColumn::DATETIME_LASTVIEW, tr("last view")}};
    d->allproperty = {{TreeColumn::TITLE, tr("title")},
                      {TreeColumn::SIZE, tr("file size")},
                      {TreeColumn::DURATION, tr("duration")},
                      {TreeColumn::URL, tr("URL")},
                      {TreeColumn::DATETIME_UPDATED, tr("update")},
                      {TreeColumn::DATETIME_LASTVIEW, tr("last view")}};

    for (auto i : d->availproperty.keys()) {
        binfo("d.keys: {} ", static_cast<int>(i));
    }
}

void EpisodeTreeModel::setupModelData(PodcastChannel *pod) {
    beginResetModel();
    d->reset();
    d->pod = pod;
    endResetModel();
}

QModelIndex EpisodeTreeModel::index(int row, int column,
                                    const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return (row >= 0 && row < this->rowCount())
               ? createIndex(row, column, d->pod->m_episodes[row])
               : QModelIndex();
}

QModelIndex EpisodeTreeModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

// TODO: fetchMore api.
// TODO: use std::accumulate from c++20
int EpisodeTreeModel::rowCount(const QModelIndex &parent) const {
    if (d->pod == nullptr || parent.isValid()) return 0;
    return std::min<int>(d->maxRow, d->pod->m_episodes.count());
}

int EpisodeTreeModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return d->availproperty.count();
}

TreeColumn EpisodeTreeModel::columnType(int column) const {
    return d->availproperty.keys().at(column);
}

QVariant EpisodeTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    auto &&ep = d->pod->m_episodes[index.row()];
    // auto realIndex = d->availproperty.keys().at(index.column());
    auto realIndex = columnType(index.column());

    if (role == Qt::DecorationRole) {
        switch (index.column()) {
            case 0:
                return QIcon::fromTheme("nvim");
            case 1:
                return QIcon::fromTheme("mpv");
            default:
                break;
        }
    }

    if (role == Qt::ToolTipRole) {
        return ep->description;
    }

    if (role != Qt::DisplayRole && role != Qt::UserRole) return QVariant();

    switch (realIndex) {
        case TreeColumn::TITLE:
            return ep->title;
        case TreeColumn::UPDATETIME:
            return ep->updatetime_str;
            break;
        case TreeColumn::SIZE:
            if (role == Qt::UserRole) {
                return ep->filesize;
            } else {
                return size_human(ep->filesize);
            }
        case TreeColumn::DURATION:
            if (role == Qt::UserRole) {
                return ep->duration;
            } else {
                return int2hms(ep->duration);
            }

        case TreeColumn::DATETIME_UPDATED:
            return ep->updatetime_str;
            break;
        case TreeColumn::DATETIME_LASTVIEW:
            break;
        case TreeColumn::LOCATION:
            return ep->location;
            break;
        case TreeColumn::URL:
            return ep->url;
            break;

        default:
            break;
    }

    return QVariant();
}

QVariant EpisodeTreeModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section > 0 && section < d->availproperty.keys().length()) {
            auto key = d->availproperty.keys().at(section);
            return d->availproperty[key];
        }
    }
    return QVariant();
}

bool EpisodeTreeSortFilterModel::lessThan(const QModelIndex &left,
                                          const QModelIndex &right) const {
    if (!left.isValid() || !right.isValid()) {
        berror("sort, index not valid: ({}, {}) vs ");
        return true;
    }
    auto leftdata = sourceModel()->data(left, Qt::UserRole);
    auto rightdata = sourceModel()->data(right, Qt::UserRole);
    if (leftdata.userType() == QMetaType::Int) {
        return leftdata.toInt() < rightdata.toInt();
    } else {
        return QString::localeAwareCompare(leftdata.toString(), rightdata.toString()) < 0;
    }

    return true;
}
