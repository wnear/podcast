#include "podmodel.h"
#include "log.h"
#include <QModelIndex>

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QIcon>
#include <QFile>

PodModel::PodModel(QList<PodcastChannel *> &pods, QObject *parent)
    : QAbstractListModel(parent), m_data(pods) {}

QVariant PodModel::data(const QModelIndex &index, int role) const {
    auto x = index.row();
    if (x < 0 || x >= m_data.length()) return {};

    auto &&cur = m_data[x];
    switch (role) {
        case Qt::DisplayRole:{
            QString res;
            res = cur->m_feedTitle;
            if(cur->size()){
                res += QString("(%1)").arg(cur->m_episodes.size());
            }
            return res;
        }
        case UrlRole:
            return cur->m_feedUrl;
        case Qt::DecorationRole:
            if(QFile(cur->coverfile()).exists()){
                return QIcon(QPixmap::fromImage(QImage(cur->coverfile())));
            } else  {
                //TODO: default icon.
                return QIcon::fromTheme("mpv");
            }
        default:
            break;
    }
    return {};
}

int PodModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_data.length();
}
