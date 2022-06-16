#pragma once

#include <QTreeView>
class QWidget;
class PodData;


class EpisodeTreeView: public QTreeView
{
    public:
      EpisodeTreeView(QWidget *parent = nullptr);
      ~EpisodeTreeView(){}
      void setPod(PodData *pod);
    private:
        class EpisodeTreeViewPrivate *d;
};
