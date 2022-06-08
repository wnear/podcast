#ifndef _EPISODE_WIDGET_H
#define _EPISODE_WIDGET_H

#include <QFrame>
#include "episodemodel.h"

class EpisodeWidget :public QFrame
{
    Q_OBJECT

    public:
        EpisodeWidget(EpisodeData& data, QWidget *parent = nullptr);
        ~EpisodeWidget(){}
        QString msg() const;
    public slots:
        void onCustomContextMenuRequested(const QPoint &);

    private:
        struct Private;
        Private *d;
        int id;
        EpisodeData &m_data;
};

#endif
