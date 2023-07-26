#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

#include "episodedata.h"
#include "podcastchannel.h"

enum FindChannelResult { FIND, NOTFIND, FIND_PARTLY, IN_VALID, SQLError };

class SQLManager {
  public:
    SQLManager();
    ~SQLManager() {}
    static SQLManager *instance() {
        static SQLManager *instance = new SQLManager;
        return instance;
    }

    void init();
    void init_id();
    void loadChannels(QList<PodcastChannel *> &result);
    void loadEpisodes(PodcastChannel *channel);

    void addChannel(const QString &title, const QString &url);
    void addChannels(QList<std::pair<QString, QString>> ch,
                     QList<std::tuple<int, QString, QString>> &feedback);
    // find channel in db, prompt dialog asking abort or edit existing.
    FindChannelResult findChannel(const QString &title, const QString &url);
    void updateChannelData(int channelid, PodcastChannel *ch);
    void updateChannelTiTleUrl(int channelid, PodcastChannel *ch);

    void addEpisode(int channelid, EpisodeData *ep);

    // TODO: history, notes, play-position, etc.
    // if have ever been played, will have one , else , no need.
    void updateEpisode();

    void close() { m_sqldb.close(); }
    bool runScript(QString fileName);
    bool runScript(QFile *file, QSqlQuery *query);

    bool logSqlError(QSqlError error, bool fatal = false);
    void checkReturn(bool ok, QSqlQuery &q, const QString &msg = "", int line = -1);

  private:
    QSqlDatabase m_sqldb;
    int m_channelID = 0;
    int m_epID = 0;
};

// TODO: move logic to seperate class.
class SQLChannelManager {};

class SQLEpisodeManager {};
