#ifndef _EPISODE_MODEL_H
#define _EPISODE_MODEL_H

#include <QString>
#include <QDateTime>
#include <QAbstractListModel>
#include <QUrl>

struct EpisodeData
{
        QString title;
        QString author;
        QUrl url;
        QString description;
        QDateTime updatetime;
        QString updatetime_str;
        int duration;

        QString location;
        bool cached;
};
class EpisodeModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        enum MORERULE {
            UrlRole = Qt::UserRole,

        };
        EpisodeModel (const QList<EpisodeData> &pods, QObject *parent = nullptr);

        QVariant data(const QModelIndex &index, int role) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    private:
        const QList<EpisodeData> &m_data;
};
#endif
