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
    QString datapath() const;
    void read_opml(const QString &filename);
    void write_opml(const QString &filename);

    void savepod(QString title, QString url);

    bool load();
    bool save();

    QString datapath(PodcastChannel &name) const;
    bool load(PodcastChannel &name);
    bool save(PodcastChannel &name);
    void podLoad(PodcastChannel &pod);
    void podUpdate(PodcastChannel &pod);
    bool parsexml(PodcastChannel &pod);
    bool updatexml(PodcastChannel &pod);

  private:
    class Private;
    Private *d;
    QList<PodcastChannel *> m_pods;
};

#endif  // !__PODCAST_H
