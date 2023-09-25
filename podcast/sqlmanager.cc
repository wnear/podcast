#include "sqlmanager.h"
#include "quuid.h"
#include "utils.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>

SQLManager::SQLManager() {
    QString m_location = util::datadir().absoluteFilePath("localpod.sqlite3");

    // Open the database connection
    m_sqldb = QSqlDatabase::addDatabase("QSQLITE");
    m_sqldb.setDatabaseName(m_location);
    bool ok = m_sqldb.open();
    if (!ok)
        qFatal(
            "Fatal error establishing a connection with Vibrato's sqlite3 database. :(");
}

void SQLManager::init() {
    // Check if 'notes' table exists. If not, create it and import tutorial note.
    // Create any tables that are non-existent.
    runScript(":sql/create.sql");
    init_id();
}

void SQLManager::init_id() {
    m_channelID = 0;
    m_epID = 0;
    QSqlQuery q;
    q.exec("select id from localchannel");
    if (q.last()) {
        m_channelID = q.value("id").toInt();
    }
    q.exec("select id from episodes");
    if (q.last()) {
        m_epID = q.value("id").toInt();
    }
    qDebug() << QString("init id res: channel_id max (%1), episode_id max (%2)")
                    .arg(m_channelID)
                    .arg(m_epID);
}

bool SQLManager::runScript(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to load SQL file" << fileName;
        return false;
    }
    QSqlQuery query;
    return runScript(&file, &query);
}

bool SQLManager::runScript(QFile *file, QSqlQuery *query) {
    QString contents = file->readAll();

    // Remove sql comments and
    contents = contents.replace(QRegularExpression("--[^\\n]+\\n"), "\n");
    // Remove unnecesary blank lines
    contents = contents.replace(QRegularExpression("\\n\\n+"), "\n");

    QStringList queryList = contents.split(";");

    for (QString line : queryList) {
        line = line.trimmed();
        if (line.isEmpty()) continue;

        bool success = query->exec(line);
        if (!success) {
            qDebug() << "current line is:" << line;
            logSqlError(query->lastError());
        }
    }

    return query->isActive();
}

bool SQLManager::logSqlError(QSqlError error, bool fatal) {
    if (!error.isValid()) return true;
    QString status = "SQL Error";
    if (fatal) status = "FATAL SQL Error";

    QString msg =
        QString("[%1] %2 %3").arg(status, error.driverText(), error.databaseText());

    // Print the message. Must convert it from QString to char*.
    if (fatal)
        qFatal("%s", msg.toLatin1().constData());
    else
        qWarning("%s", msg.toLatin1().constData());

    return false;
}

void SQLManager::loadChannels(QList<PodcastChannel *> &result) {
    QSqlQuery q;
    QString cmdstr = QString("select id, title,feedurl from localchannel");
    q.prepare(cmdstr);
    auto ok = q.exec();
    checkReturn(ok, q, __PRETTY_FUNCTION__, __LINE__);
    if (!ok) {
        return;
    }

    // TODO: load more data fields to channel.
    while (q.next()) {
        auto id = q.value("id").toInt();
        auto title = q.value("title").toString();
        auto feed = q.value("feedurl").toString();
        result.push_back(new PodcastChannel(title, feed));
        result.back()->channelID = id;
    }
}

void SQLManager::loadEpisodes(PodcastChannel *channel) {
    QSqlQuery q;
    QString cmdstr =
        QString(
            "select id, title, mediafileUrl, "
            "cached,cacheLocation,description,filesize,date_published,"
            "duration, play_position from episodes where channelid = %1 order by date_published "
            "limit 30")
            .arg(channel->channelID);
    q.prepare(cmdstr);
    auto ok = q.exec();
    checkReturn(ok, q, __PRETTY_FUNCTION__, __LINE__);
    if (!ok) {
        return;
    }

    // TODO: load more data fields.
    while (q.next()) {
        EpisodeData *x = new EpisodeData;
        x->id = q.value("id").toInt();
        x->title = q.value("title").toString();
        x->url = QUrl::fromEncoded(q.value("mediafileUrl").toString().toLatin1());
        x->cached = q.value("cached").toBool();
        x->location = q.value("cacheLocation").toString();
        x->description = q.value("description").toString();
        x->filesize = q.value("filesize").toInt();
        x->updatetime = q.value("date_published").toDateTime();
        x->play_position = q.value("play_position").toInt();
        x->setUpdatetime();
        x->duration = q.value("duration").toInt();
        // TODO: episode data init should be in one loadFromJsoon,
        x->calculateCurrentSize();
        channel->m_episodes.push_back(x);
    }
}

// TODO: merge findchannel into addchannel, return false/true.
// prompt dialog is make from here.
int SQLManager::addChannel(const QString &title, const QString &url) {
    QSqlQuery q;
    QString cmdstr =
        QString("insert into localchannel (id, title, feedurl) values (%1, %2, %3)")
            .arg(":id")
            .arg(":title")
            .arg(":url");
    q.prepare(cmdstr);

    m_channelID++;
    q.bindValue(":id", m_channelID);
    q.bindValue(":title", title);
    q.bindValue(":url", url);

    auto ok = q.exec();
    checkReturn(ok, q, "add channel");
    if (!ok) {
        return -1;
    } else {
        return m_channelID;
    }
}

void SQLManager::addChannels(QList<PodcastChannel *> &container,
                             QList<channel_item_t> &channels,
                             QList<std::pair<channel_item_t, channel_item_t>> &conflict) {
    channel_item_t match;
    for (auto [title, url] : channels) {
        auto res = findChannel(title, url, match);
        switch (res) {
            case SQLError:
                assert(0);
            case FIND:
                break;
            case FIND_CONFLICT:
                // dialog.
                conflict.emplace_back(std::make_pair(title, url), match);
                break;
            case NOTFIND: {
                auto id = SQLManager::instance()->addChannel(title, url);
                if (id != -1) {
                    container.push_back(new PodcastChannel(title, url));
                    container.back()->channelID = id;
                } else {
                    assert(0);
                }
                break;
            }
            case IN_VALID:
                break;
        }
    }
}

// TODO: dialog ask what to do for existing one.
//  if tile==title, url==url, skip
//  if one of the comparsion is true, ask user to decide.
FindChannelResult SQLManager::findChannel(const QString &title, const QString &url,
                                          channel_item_t &match) {
    if (url.isEmpty()) {
        qDebug() << "url is empty";
        return IN_VALID;
    }
    // res = false;
    // res |= find with title,
    // res |= find with url,
    QSqlQuery q;
    QString cmdstr;
    if (title.isEmpty()) {
        cmdstr =
            QString("select title, feedurl from localchannel where feedurl = :arg_url");
        q.prepare(cmdstr);
        q.bindValue(":arg_url", url);
        auto ok = q.exec();
        checkReturn(ok, q, "find with url", __LINE__);
        if (not ok) return SQLError;
        while (q.next()) {
            return FIND;
        }
        return NOTFIND;
    } else {
        cmdstr = QString(
            "select title, feedurl from localchannel where title = :arg_title or feedurl "
            "= :arg_url");
        q.prepare(cmdstr);
        q.bindValue(":arg_title", title);
        q.bindValue(":arg_url", url);
        auto ok = q.exec();
        checkReturn(ok, q, "find with url", __LINE__);
        if (not ok) return SQLError;
        // QString ret_title, ret_url;
        auto &match_title = match.first;
        auto &match_url = match.second;
        if (q.next()) {
            match_title = q.value("title").toString();
            match_url = q.value("feedurl").toString();
            if (match_title == title and match_url == url) {
                return FIND;
            } else {
                return FIND_CONFLICT;
            }
        }
        return NOTFIND;
    }
    return IN_VALID;
}

void SQLManager::reinitEpisode(EpisodeData *ep) {
    assert(ep->id != -1);
    QSqlTableModel model;
    model.setTable("episodes");
    model.setFilter(QString("id = \"%1\"").arg(ep->id));
    model.select();

    if (model.rowCount() > 1)
        qWarning() << "[Duplicate Tag SQLite3 Warning!] Found" << model.rowCount() << "of"
                   << ep->title;

    QSqlRecord tagInDB = model.record(0);
    tagInDB.setValue("title", ep->title);
    model.setRecord(0, tagInDB);

    // return logSqlError(model.lastError());
}

// TODO: check.
void SQLManager::addEpisode(int channelid, EpisodeData *ep) {
    assert(channelid != -1);
    QSqlQuery q;
    QString cmdstr =
        QString(
            "insert into episodes (id, title, mediafileUrl, channelid, description, "
            "cached, cacheLocation, filesize, date_published, duration"
            ") values (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10)")
            .arg(":id")
            .arg(":title")
            .arg(":url")
            .arg(":channel")
            .arg(":description")
            .arg(":cached")
            .arg(":cachelocation")
            .arg(":filesize")
            .arg(":date_published")
            .arg(":duration");
    q.prepare(cmdstr);

    m_epID++;
    q.bindValue(":id", m_epID);
    ep->id = m_epID;
    q.bindValue(":title", ep->title);
    q.bindValue(":url", ep->url);
    q.bindValue(":channel", channelid);
    q.bindValue(":description", ep->description);
    q.bindValue(":cached", ep->actualSize == ep->filesize);
    q.bindValue(":cachelocation", ep->location);
    q.bindValue(":filesize", ep->filesize);
    q.bindValue(":date_published", ep->updatetime);
    q.bindValue(":duration", ep->duration);

    // q.bindValue(":title", title);
    // q.bindValue(":url", url);

    auto ok = q.exec();
    checkReturn(ok, q, __PRETTY_FUNCTION__);
}

void SQLManager::checkReturn(bool ok, QSqlQuery &q, const QString &msg, int line) {
    if (not ok) {
        if (line == -1)
            qDebug() << QString("[sql error]: %1").arg(msg);
        else
            qDebug() << QString("[sql error at L%1]: %2").arg(line).arg(msg);
        logSqlError(q.lastError());
    }
}

void SQLManager::updateChannelData(int channelid, PodcastChannel *ch) {}

void SQLManager::updateChannelTiTleUrl(int channelid, PodcastChannel *ch) {}

void SQLManager::clearEpisodes(int channelid) {
    QSqlQuery q;
    QString cmdstr = QString("delete from episodes where channelid = %1").arg(channelid);
    q.prepare(cmdstr);
    auto ok = q.exec();
    checkReturn(ok, q, __PRETTY_FUNCTION__, __LINE__);
    if (!ok) {
        return;
    }
}

void SQLManager::updateEpisodePlayposition(EpisodeData *ep){
    QSqlQuery q;
    QString cmdstr = QString("UPDATE episodes SET play_position = %1 WHERE id = %2").arg(ep->play_position).arg(ep->id);
    q.prepare(cmdstr);
    auto ok = q.exec();
    checkReturn(ok, q, __PRETTY_FUNCTION__, __LINE__);
    if (!ok) {
        return;
    }
};
