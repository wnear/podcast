#ifndef __PODCAST_H
#define __PODCAST_H

#include <QWidget>
#include <QDateTime>
#include <QUrl>

// #include "podcastchannel.h"

class PodcastChannel;
class EpisodeData;
class EpisodeDetailWidget;

class Podcast : public QWidget {
    Q_OBJECT

  public:
    Podcast(QWidget *parent = nullptr);
    ~Podcast(){}

    void importdlg();
    void exportdlg();
    QWidget *detail() const;
    EpisodeDetailWidget *ep_detail() const;

  signals:
    void requestPlay(EpisodeData* ep);
    void requestDetail(EpisodeData* ep);

  private:
    void import_opml(const QString &filename);
    void export_opml(const QString &filename);

    bool load();
    bool save();
    void about_to_close(){
        //TODO: save.
    }

    void pod_load_episodes(PodcastChannel &pod);

  private:
    class Private;
    Private *d;
    QList<PodcastChannel *> m_pods;

    // TODO: to save/restore channel upon app startup.
    QString m_active_channel;
};

#endif  // !__PODCAST_H
