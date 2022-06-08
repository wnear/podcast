#ifndef _EPISODE_WIDGET_H
#define _EPISODE_WIDGET_H

#include <QWidget>
#include "episodemodel.h"

class EpisodeWidget :public QWidget
{
    Q_OBJECT

    public:
        EpisodeWidget(EpisodeData& data, QWidget *parent = nullptr);
        ~EpisodeWidget(){}
        QString msg() const;
    private:
        int id;
        EpisodeData &m_data;
};

#endif
