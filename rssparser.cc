#include "rssparser.h"

#include <QIODevice>
#include <QDir>
#include "poddata.h"
#include "log.h"
#include "episodedata.h"
#include <QXmlStreamReader>
#include <QDebug>

#include "utils.h"
#include <algorithm>

#include <QUrl>

namespace
{

    void backToParent(QXmlStreamReader *reader) {
        int level = 1;

        while(level != 0 && !reader->atEnd()) {
            switch(reader->readNext()) {
                case QXmlStreamReader::StartElement: ++level; break;
                case QXmlStreamReader::EndElement: --level; break;
                default: break;
            }
        }
    }

bool parseUntilElement(QXmlStreamReader *reader, const QString &name)
{

  while (!reader->atEnd()) {
    QXmlStreamReader::TokenType type = reader->readNext();
    if (type == QXmlStreamReader::StartElement && reader->name() == name) {
      return true;
    }
  }
  return false;
}
}


RssParser::RssParser(QIODevice *f, PodData *pod): m_pod(pod), m_file(f)
{
    binfo("xml for parse: {} big.", size_human(f->size()));
    reader = new QXmlStreamReader{m_file};
}

bool RssParser::parse()
{
    m_pod->episodes.clear();
    bool ret = true;
    if (!m_file->isOpen()){
        if(m_file->open(QIODevice::ReadOnly) == false){
            qDebug()<<"open file error in rssparser::parse()";
            return false;
        }
    }

    ret = parseUntilElement(reader, "channel");
    if(ret == false) {
        qDebug()<<"Channel not found, early exit.";
        return false;
    }

    while(!reader->atEnd()) {
        QXmlStreamReader::TokenType type = reader->readNext();

        switch(type) {
            case QXmlStreamReader::StartElement: {
                const QString name = reader->name().toString();
                const QString lower_namespace = reader->namespaceUri().toString().toLower();
                //binfo("parse, header, get field of name: {}", name);
                /*
                if (name == "title") {
                ret->set_title(reader->readElementText());
                }
                else if (name == "link" && lower_namespace.isEmpty()) {
                ret->set_link(QUrl::fromEncoded(reader->readElementText().toLatin1()));
                }
                else if (name == "description") {
                ret->set_description(reader->readElementText());
                }
                else if (name == "owner" && lower_namespace == kItunesNamespace) {
                ParseItunesOwner(reader, ret);
                }
                else if (name == "image") {
                ParseImage(reader, ret);
                }
                else if (name == "copyright") {
                ret->set_copyright(reader->readElementText());
                }
                else if (name == "link" && lower_namespace == kAtomNamespace && ret->url().isEmpty() && reader->attributes().value("rel").toString() == "self") {
                ret->set_url(QUrl::fromEncoded(reader->readElementText().toLatin1()));
                }
                */
                if(name == "item") { 
                    parseEpisode(); 
                } 
                else if (name == "image") {
                    auto cover_url = reader->attributes().value("href").toString();
                    m_pod->cover_url = cover_url;
                    binfo("get cover:{}", cover_url);
                    ParseImage();
                }
                else { backToParent(reader); }
                break;
            }
            case QXmlStreamReader::EndElement:
                // binfo("{} episode cnt, {}", m_pod->title.toStdString(), m_pod->episodes.length());
                return m_pod->episodes.length() > 0;

            default: break;
        }
    }

    // binfo("{} episode cnt, {}", m_pod->title.toStdString(), m_pod->episodes.length());
    return m_pod->episodes.length() > 0;
}


void RssParser::ParseImage() {

  // while (!reader->atEnd()) {
  //   QXmlStreamReader::TokenType type = reader->readNext();
  //   switch (type) {
  //     case QXmlStreamReader::StartElement: {
  //       const QString name = reader->name().toString();
  //       if (name == "url") {
  //         ret->set_image_url_large(
  //           QUrl::fromEncoded(reader->readElementText().toLatin1()));
  //       }
  //       else {
  //         Utilities::ConsumeCurrentElement(reader);
  //       }
  //       break;
  //     }

  //     case QXmlStreamReader::EndElement:
  //       return;

  //     default:
  //       break;
  //   }
  // }

}

void RssParser::parseEpisode()
{
    EpisodeData *episode = new EpisodeData;
    while(!reader->atEnd()) {
        QXmlStreamReader::TokenType type = reader->readNext();

        switch(type) {
            case QXmlStreamReader::StartElement: {
                const QString name = reader->name().toString();
                const QString lower_namespace = reader->namespaceUri().toString().toLower();

                if(name == "title") {
                    episode->title = (reader->readElementText());
                } else if(name == "description") {
                    episode->description = (reader->readElementText());
                } else if(name == "pubDate") {
                    QString date = reader->readElementText();
                    episode->updatetime_str = date;
                } else if(name == "duration" /* && lower_namespace == kItunesNamespace */) {
                    // http://www.apple.com/itunes/podcasts/specs.html
                    QStringList parts = reader->readElementText().split(':');
                    QList<int> pi;
                    std::transform(parts.begin(), parts.end(), std::back_inserter(pi), [](QString a){
                                       return a.toInt();
                                   });
                    episode->duration = std::accumulate(std::begin(pi), std::end(pi), 0, [](auto x, auto y){ return x * 60 + y; });
                    // #FIXME 
                    // why this fail...
                    //episode->duration = std::accumulate(std::begin(parts), std::end(parts), 0, [](auto x, auto y){ return x.toInt()* 60 + y.toInt(); });

                } else if(name == "enclosure") {
                    const QString type2 = reader->attributes().value("type").toString();
                    const QUrl url = QUrl::fromEncoded(
                                         reader->attributes().value("url").toString().toLatin1());

                    if(type2.startsWith("audio/") || type2.startsWith("x-audio/")) {
                        episode->url = (url);
                    }
                    // If the URL doesn't have a type, see if it's one of the obvious types
                    else if(type2.isEmpty() && (url.path().endsWith(".mp3", Qt::CaseInsensitive)
                                                || url.path().endsWith(".m4a", Qt::CaseInsensitive)
                                                || url.path().endsWith(".wav", Qt::CaseInsensitive))) {
                        episode->url = (url);
                    }
                    episode->filesize = reader->attributes().value("length").toInt();

                    backToParent(reader);
                } else if(name == "author" /*&& lower_namespace == kItunesNamespace*/) {
                    episode->author = (reader->readElementText());
                } else {
                    backToParent(reader);
                }

                break;
            }

            case QXmlStreamReader::EndElement:
                if(!episode->url.isEmpty()) {
                    auto ret = std::find_if(m_pod->episodes.begin(), m_pod->episodes.end(), [episode](auto && ep){
                                                return ep->title == episode->title;
                                            });
                    if(ret == m_pod->episodes.end()){
                        episode->location = episode->url.fileName();
                        // binfo("ending of parser, duration({}), filesize({}), location({})", episode->duration, episode->filesize, episode->location.toStdString());
                        m_pod->episodes.push_back(episode);
                        
                    }
                }

                return;

            default:
                break;
        }
    }
}
