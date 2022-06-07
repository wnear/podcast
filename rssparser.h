#ifndef __RSS_PARSER_H
#define __RSS_PARSER_H


class QIODevice;
class PodData;

class RssParser
{
public:
    RssParser(QIODevice *, PodData *);
    bool parse();
    ~RssParser();
private:

private:
    PodData *m_pod;
    QIODevice *m_file;
};


#endif
