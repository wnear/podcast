#pragma once

#include <QFrame>

class QWidget;

class EpisodeTreeWidget: public QFrame 
{
    public:
      EpisodeTreeWidget(QWidget *parent = nullptr);
      ~EpisodeTreeWidget() {}
      
    private:
        class EpisodeTreeWidgetPrivate *d;

};
