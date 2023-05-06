#ifndef __PODCAST_H
#define __PODCAST_H

#include <QWidget>
#include <QDateTime>

#include "podmodel.h"
#include "episodedata.h"

class Podcast : public QWidget {
  public:
    Podcast(QWidget *parent = nullptr);
    ~Podcast() = default;

    void importdlg();
    void exportdlg();
    QWidget *detail() const;

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
