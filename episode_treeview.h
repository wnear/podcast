#pragma once

#include <QTreeView>
class QWidget;
class PodData;


class EpisodeTreeView: public QTreeView
{
    public:
      EpisodeTreeView(QWidget *parent = nullptr);
      ~EpisodeTreeView();
      void setPod(PodData *pod);

      void onCustomContextMenuRequested(const QPoint &p);
        // play
      void mouseDoubleClickEvent(QMouseEvent *event) override;
      // into detail wgt.
      void mousePressEvent(QMouseEvent *event) override;
    protected:
      bool eventFilter(QObject *obj, QEvent *evt) override;

    private:
        class EpisodeTreeViewPrivate *d;
};
