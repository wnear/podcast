#ifndef _EPISODE_WIDGET_H
#define _EPISODE_WIDGET_H

#include <QFrame>
#include "episodedata.h"

class EpisodeWidget : public QFrame {
    Q_OBJECT

  public:
    EpisodeWidget(EpisodeData *data, QWidget *parent = nullptr);
    ~EpisodeWidget() {}
    QString msg() const;
    void setProgressText();

  public slots:
    void onCustomContextMenuRequested(const QPoint &);

  private:
    struct EpisodeWidgetPrivate *d;
    int id;
    EpisodeData *m_data{nullptr};
};

#endif
