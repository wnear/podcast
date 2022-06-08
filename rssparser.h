#ifndef __RSS_PARSER_H
#define __RSS_PARSER_H


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

private:
    PodData *m_pod;
    QIODevice *m_file;
    QXmlStreamReader *reader;
};


#endif
