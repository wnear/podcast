#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

#include "episodedata.h"

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
    void addChannel(const QString &title, const QString &url);
    // find channel in db, prompt dialog asking abort or edit existing.
    FindChannelResult findChannel(const QString &title, const QString &url);

    void addEpisode(int channelid, EpisodeData *ep);

    //TODO: history, notes, play-position, etc.
    //if have ever been played, will have one , else , no need.
    void updateEpisode();

    void close() { m_sqldb.close(); }
    bool runScript(QString fileName);
    bool runScript(QFile *file, QSqlQuery *query);

    bool logSqlError(QSqlError error, bool fatal = false) ;
    void checkReturn(bool ok, QSqlQuery &q, const QString &msg = "", int line = -1) ;

  private:
    QSqlDatabase m_sqldb;
    int m_channelID = 0;
    int m_epID = 0;
};

// TODO: move logic to seperate class.
class SQLChannelManager {};

class SQLEpisodeManager {};
