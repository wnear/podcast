#pragma once

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

enum TreeColumn {
    TITLE = 0,
    UPDATETIME,
    SIZE,
    DURATION,
    LASTVIEW,
    LOCATION,
    CNT_SENTEL
};

class PodData;

class EpisodeTreeModel : public QAbstractItemModel
{
    public:
      EpisodeTreeModel(QObject *parent = nullptr);
      ~EpisodeTreeModel() {}

      void setPod(PodData *pod);
      Qt::ItemFlags flags(const QModelIndex &index) const override;
      bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
      QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
      QModelIndex parent(const QModelIndex &child) const override;
      int rowCount(const QModelIndex &parent = QModelIndex()) const override;
      int columnCount(const QModelIndex &parent = QModelIndex()) const override;
      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
      TreeColumn columnType(int column) const;

    private:
        class EpisodeTreeModelPrivate *d;
};


class EpisodeTreeSortFilterModel : public QSortFilterProxyModel
{
    public:
      EpisodeTreeSortFilterModel(QObject *parent = nullptr):QSortFilterProxyModel(parent){}
      bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};
