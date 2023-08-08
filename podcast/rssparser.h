#pragma once

#include <QString>
#include <QList>

class QIODevice;
class PodcastChannel;
class QXmlStreamReader;
class EpisodeData;

class RssParser {
  public:
    RssParser(QIODevice *, PodcastChannel *);
    bool parse();
    ~RssParser() {}
    bool isValid();

  private:
    void parseEpisode();
    void ParseImage();

  private:
    PodcastChannel *m_podchannel;
    QList<EpisodeData*> m_episodes;
    QIODevice *m_file;
    QXmlStreamReader *reader;
};
