#include "opmlparser.h"
#include <QDomDocument>
#include <QFile>
#include "log.h"


QList<std::pair<QString, QString>> OpmlParser::parse() 
{
    elems_t res{};

    QFile xml(m_filename);
    if(xml.exists() == false || xml.open(QIODevice::ReadOnly) == false){
        berror("error to open opml file");
        return res;
    }

    QDomDocument docu;
    docu.setContent(xml.readAll());
    auto nodes = docu.elementsByTagName("outline");
    
    QString name, url;
    for(decltype(nodes.length()) i = 0; i < nodes.length(); i++) {
        auto node = nodes.item(i);
        if(node.isElement()){
            auto elnode = node.toElement();
            if(elnode.isNull())
                continue;
            name = elnode.attribute("text", "");
            url = elnode.attribute("xmlUrl", "");
            if(!url.isEmpty()){
                res.push_back({name, url});
            }
        }
    }

    return res;
}
