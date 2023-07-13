#pragma once

#include <QTreeView>
#include <QUrl>
class QWidget;
// class PodcastChannel;
class EpisodeData;

class EpisodeTreeView : public QTreeView {
    Q_OBJECT

  public:
    EpisodeTreeView(QAbstractItemModel *model, QWidget *parent = nullptr);
    ~EpisodeTreeView();

    void onCustomContextMenuRequested(const QPoint &p);
    // play
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // // into detail wgt.
    // void mousePressEvent(QMouseEvent *event) override;

  protected:
    // bool eventFilter(QObject *obj, QEvent *evt) override;
  signals:
    // void requestPlay(QUrl media);
    void requestPlay(EpisodeData *ep);
    void requestDetail(EpisodeData *ep);


  private:
    class EpisodeTreeViewPrivate *d;
};
