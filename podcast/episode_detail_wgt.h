#pragma once

#include <QWidget>
#include "episodedata.h"

class EpisodeDetailWidget : public QWidget {
    Q_OBJECT

  public:
    EpisodeDetailWidget(QWidget *parent = nullptr);
    static EpisodeDetailWidget *instance() {
        static EpisodeDetailWidget ins;
        return &ins;
    }
    void setData(EpisodeData *data);
    ~EpisodeDetailWidget();
signals:
    void requestEpisodePosition(EpisodeData *ep, QString pos);
    void requestOpenLink(const QUrl &url);


  private:
    class EpisodeDetailPrivate *d;
    EpisodeData *m_cur_episode{nullptr};
};
