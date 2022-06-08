#ifndef _EPISODE_LIST_WIDGET_H
#define _EPISODE_LIST_WIDGET_H

#include <QWidget>

class PodData;

class EpisodeListWidget : public QWidget 
{
    Q_OBJECT

    public:
        EpisodeListWidget(QWidget *parent = nullptr);
        void setPod(PodData* pod);
    private:
        QWidget *scrollWidget(); 
        struct Private;
        Private *d;
    };


#endif
