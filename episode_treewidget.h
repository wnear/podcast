#pragma once

#include <QFrame>

class QWidget;
class PodData;

// Qt model view: 
// - widget is the whole controller, contains the
//   view to show the model, also some other button to control the 
//   view, like sort and selection. 
// - view, is the view. may be also the itemdelegate, for painting. 
// - model, the place to parse index, get data, also deal with 
//   sort/filter and selection.

class EpisodeTreeWidget: public QFrame 
{
    public:
      EpisodeTreeWidget(QWidget *parent = nullptr);
      ~EpisodeTreeWidget() {}
      void setPod(PodData *pod);

      // TODO:
      // chain: click on pod
      // ==> TreeWidget get data
      // ==> TreeView get data
      // ==> TreeModel reset data.
      // How did others deal with similar scenerios.
      // how to be more elegant?
      void setData();
    private:
        class EpisodeTreeWidgetPrivate *d;

};