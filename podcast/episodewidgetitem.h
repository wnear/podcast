#ifndef _EPISODE_WIDGET_H
#define _EPISODE_WIDGET_H

#include <QFrame>
#include "episodedata.h"

class EpisodeWidgetItem : public QFrame {
    Q_OBJECT

  public:
    EpisodeWidgetItem(EpisodeData *data, QWidget *parent = nullptr);
    ~EpisodeWidgetItem() {}
    QString msg() const;
    void setProgressText();

  public slots:
    void onCustomContextMenuRequested(const QPoint &);

  private:
    struct EpisodeWidgetItemPrivate *d;
    int id;
    EpisodeData *m_data{nullptr};
};

#endif
