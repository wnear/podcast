#pragma once

#include <QString>

class QIODevice;
class PodcastChannel;
class QXmlStreamReader;

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
    PodcastChannel *m_pod;
    QIODevice *m_file;
    QXmlStreamReader *reader;
};
