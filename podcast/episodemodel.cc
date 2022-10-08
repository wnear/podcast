
#include "episodemodel.h"
#include <QModelIndex>

EpisodeModel::EpisodeModel (const QList<EpisodeData> &pods, QObject *parent)
   : QAbstractListModel(parent),m_data(pods) 
{

}

QVariant EpisodeModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return {};
    auto x = index.row();
    if(x < 0 || x >= m_data.length())
        return {};
    auto && cur = m_data[x];
    switch(role){
        case Qt::DisplayRole:
            return cur.title;
        case UrlRole:
            return cur.url;
        default:
            return "no";
    }
}

int EpisodeModel::rowCount(const QModelIndex &parent ) const
{
    return m_data.length();
}
