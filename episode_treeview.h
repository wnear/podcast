#pragma once

#include <QTreeView>
class QWidget;

class EpisodeTreeView: public QTreeView
{
    public:
      EpisodeTreeView(QWidget *parent = nullptr);
      ~EpisodeTreeView(){}

};
