#ifndef __PODCAST_H
#define __PODCAST_H

#include <QWidget>
#include <QDateTime>

#include "podmodel.h"
#include "episodemodel.h"


class Podcast : public QWidget
{
    public:
        Podcast(QWidget *parent = nullptr);
        ~Podcast() = default;

        void importdlg();
        void exportdlg();
        QWidget *detail() const;

    private:
        QString datapath() const;
        void read_opml(const QString &filename);
        void write_opml(const QString &filename);

        void savepod(QString title, QString url);

        bool load();
        bool save();

        QString datapath( PodData &name) const;
        bool load(PodData &name);
        bool save(PodData &name);
        void podLoad(PodData &pod);
        void podUpdate(PodData &pod);
        
    private:
        class Private;
        Private *d;
        QList<PodData> m_pods;

    
};


#endif // !__PODCAST_H
