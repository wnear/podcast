#pragma once

#include <QSqlDatabase>
#include <QFile>

class SQLManager {
  public:
    SQLManager();
    ~SQLManager() {}
    static SQLManager *instance() {
        static SQLManager *instance = new SQLManager;
        return instance;
    }

    void init();
    void addChannel(const QString &title, const QString &url);
    // find channel in db, prompt dialog asking abort or edit existing.
    bool findChannel(const QString &title, const QString &url);
    void updateChannel(const QString &feedurl){}

    void addEpisode(const QString &feedurl);
    void updateEpisode();
    void close() { m_sqldb.close(); }
    bool runScript(QString fileName);
    bool runScript(QFile *file, QSqlQuery *query);

    bool logSqlError(QSqlError error, bool fatal = false);

  private:
    QSqlDatabase m_sqldb;
    int m_channelID = 0;
    int m_epID = 0;
};

// TODO: move logic to seperate class.
class SQLChannelManager {};

class SQLEpisodeManager {};
