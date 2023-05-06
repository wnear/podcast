#pragma once

#include <QTreeView>
class QWidget;
// class PodcastChannel;
class EpisodeData;

class EpisodeTreeView : public QTreeView {
  public:
    EpisodeTreeView(QAbstractItemModel *model, QWidget *parent = nullptr);
    ~EpisodeTreeView();

    void onCustomContextMenuRequested(const QPoint &p);
    // play
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    // into detail wgt.
    void mousePressEvent(QMouseEvent *event) override;

  protected:
    bool eventFilter(QObject *obj, QEvent *evt) override;
  signals:
    void showEpDetail(EpisodeData *data);

  private:
    class EpisodeTreeViewPrivate *d;
};
