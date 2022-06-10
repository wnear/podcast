#ifndef _POD_MODEL_H
#define _POD_MODEL_H

#include "episodemodel.h"

#include <QString>
#include <QDateTime>
#include <QAbstractListModel>


constexpr char c_podcast_localxml[] = "podcast.xml";

struct PodData
{

        PodData(const QString &x, const QString &y){
            title = x;
            url  = y;
        }
        bool isValid(){
            return !(title.isEmpty() || url.isEmpty() );
        }
        QString title;
        QString url;
        int job_id{-1};
        QString location;
        QString xmllocation;
        QDateTime lastEpisodeUpdate;
        QString language;
        int episodeCount;
        int episodeDuratinSum;
        QList<EpisodeData> episodes;
};

class PodModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        enum MORERUlE{
            UrlRole = Qt::UserRole,
        };
        PodModel (QList<PodData> &pods, QObject *parent = nullptr);

        QVariant data(const QModelIndex &index, int role) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        void resetData(QList<PodData> &pods){m_data = pods;}
        ~PodModel() = default;

    private:
        QList<PodData> &m_data;
};


#endif
