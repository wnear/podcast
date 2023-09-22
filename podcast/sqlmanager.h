#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

#include "episodedata.h"
#include "podcastchannel.h"
#include "opmlparser.h"

enum FindChannelResult { FIND, NOTFIND, FIND_CONFLICT, IN_VALID, SQLError };

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

    int addChannel(const QString &title, const QString &url);
    void addChannels(QList<PodcastChannel*> &container, QList<channel_item_t> &to_add,
                     QList<std::pair<channel_item_t, channel_item_t>> &conflict);
    // find channel in db, prompt dialog asking abort or edit existing.
    FindChannelResult findChannel(const QString &title, const QString &url, channel_item_t & res);
    void updateChannelData(int channelid, PodcastChannel *ch);
    void updateChannelTiTleUrl(int channelid, PodcastChannel *ch);

    // int findEpisode(int channelid, int episodeid);
    // int findEpisode(int channelid, const QString &title);

    void addEpisode(int channelid, EpisodeData *ep);
    void clearEpisodes(int channelid);

    //NOTE: dev only, some data field may not be inited in early stage of the dev.
    void reinitEpisode(EpisodeData *ep);

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
