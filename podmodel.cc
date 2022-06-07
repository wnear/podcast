#include "podmodel.h"
#include <QModelIndex>

#include <QDebug>

PodModel::PodModel (QList<PodData> &pods, QObject *parent)
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
            return cur.title;
        case UrlRole:
            return cur.url;
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

