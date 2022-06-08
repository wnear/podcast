#ifndef _EPISODE_WIDGET_H
#define _EPISODE_WIDGET_H

#include <QWidget>

class EpisodeWidget :public QWidget
{
    Q_OBJECT

    public:
        EpisodeWidget(QWidget *parent = nullptr);
        ~EpisodeWidget(){}
        QString msg() const;
    private:
        int id;
};

#endif
