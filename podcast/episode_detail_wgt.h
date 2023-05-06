#pragma once

#include <QWidget>
#include "episodedata.h"

class EpisodeDetail : public QWidget {
  public:
    EpisodeDetail(QWidget *parent = nullptr);
    static EpisodeDetail *instance() {
        static EpisodeDetail ins;
        return &ins;
    }
    void setData(EpisodeData *data);
    ~EpisodeDetail();

  private:
    class EpisodeDetailPrivate *d;
    EpisodeData *m_data{nullptr};
};
