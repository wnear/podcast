#include "rssparser.h"

#include <QIODevice>
#include "podmodel.h"
#include <QXmlStreamReader>


RssParser::RssParser(QIODevice *f, PodData *pod):m_pod(pod), m_file(f)
{ }

bool RssParser::parse()
{
    QXmlStreamReader reader(m_file);
    while(!reader.atEnd()) {
        reader.readNext();
    }
    return true;
}
