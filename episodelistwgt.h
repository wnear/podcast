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
        QString current() const;
        void refresh();
        void loadmore(){
            m_count += 5;
        }
    private:
        PodData *cur{nullptr};
        QWidget *scrollWidget(); 
        struct EpisodeListWidgetPrivate *d;
        int m_count = 10;
    };


#endif
