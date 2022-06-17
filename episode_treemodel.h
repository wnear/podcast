#pragma once

#include <QAbstractItemModel>

enum TreeColumn {
    TITLE = 0,
    UPDATETIME,
    SIZE,
    DURATION,
    LASTVIEW,
    CNT_SENTEL
};

class PodData;

class EpisodeTreeModel : public QAbstractItemModel
{
    public:
      EpisodeTreeModel(QObject *parent = nullptr);
      ~EpisodeTreeModel() {}

      void setPod(PodData *pod);
      bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
      QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
      QModelIndex parent(const QModelIndex &child) const override;
      int rowCount(const QModelIndex &parent = QModelIndex()) const override;
      int columnCount(const QModelIndex &parent = QModelIndex()) const override;
      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    private:
        class EpisodeTreeModelPrivate *d;
};
