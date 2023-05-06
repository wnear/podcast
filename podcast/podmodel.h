#ifndef _POD_MODEL_H
#define _POD_MODEL_H

#include "episodedata.h"
#include "podcastchannel.h"

#include <QString>
#include <QDateTime>
#include <QAbstractListModel>

class PodModel : public QAbstractListModel {
    Q_OBJECT

  public:
    enum MORERUlE {
        UrlRole = Qt::UserRole,
    };
    PodModel(QList<PodcastChannel *> &pods, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    void resetData(QList<PodcastChannel *> &pods) { m_data = pods; }
    ~PodModel() = default;

  private:
    QList<PodcastChannel *> &m_data;
    int m_viewMode;
};

#endif
