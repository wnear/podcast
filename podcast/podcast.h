#ifndef __PODCAST_H
#define __PODCAST_H

#include <QWidget>
#include <QDateTime>
#include <QUrl>

// #include "podcastchannel.h"

class PodcastChannel;

class Podcast : public QWidget {
    Q_OBJECT

  public:
    Podcast(QWidget *parent = nullptr);
    ~Podcast() = default;

    void importdlg();
    void exportdlg();
    QWidget *detail() const;

  signals:
    void requestPlay(QUrl media);

  private:
    void read_opml(const QString &filename);
    void write_opml(const QString &filename);


    bool load();
    bool save();

    void pod_load_episodes(PodcastChannel &pod);

  private:
    class Private;
    Private *d;
    QList<PodcastChannel *> m_pods;
};

#endif  // !__PODCAST_H
