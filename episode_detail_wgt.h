#pragma once

#include <QWidget>


class EpisodeDetail : public QWidget 
{
    public:
      EpisodeDetail(QWidget *parent = nullptr);
        static EpisodeDetail *instance (){
            static EpisodeDetail ins;
            return &ins;
        }
      ~EpisodeDetail();
    private:
        class EpisodeDetailPrivate *d;
};
