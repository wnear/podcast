#include "podmodel.h"
#include "log.h"
#include <QModelIndex>

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QIcon>
#include <QFile>


PodModel::PodModel (QList<PodcastChannel *> &pods, QObject *parent)
   : QAbstractListModel(parent),m_data(pods)
{
}

QVariant PodModel::data(const QModelIndex &index, int role) const
{
    auto x = index.row();
    if(x < 0 || x >= m_data.length()) return {};

    auto && cur = m_data[x];
    switch(role){
        case Qt::DisplayRole:
            return cur->title;
        case UrlRole:
            return cur->url;
        case Qt::DecorationRole:
                 return QIcon::fromTheme("mpv");
            // if(QFile(cur->coverfile()).exists()){
            //     return QIcon(QPixmap::fromImage(QImage(cur->coverfile())));
            // } else  {
            //     return QIcon::fromTheme("mpv");
            // }
        default:
            break;
    }
    return {};
}

int PodModel::rowCount(const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_data.length();
}

