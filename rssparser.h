#pragma once

#include <QString>

class QIODevice;
class PodData;
class QXmlStreamReader;

class RssParser
{
public:
    RssParser(QIODevice *, PodData *);
    bool parse();
    ~RssParser(){}
private:
    void parseEpisode();
    void ParseImage();

private:
    PodData *m_pod;
    QIODevice *m_file;
    QXmlStreamReader *reader;
};


