#include "sqlmanager.h"
#include "utils.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>

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

// TODO: merge findchannel into addchannel, return false/true.
// prompt dialog is make from here.
void SQLManager::addChannel(const QString &title, const QString &url) {
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

    if (!ok) {
        qDebug() << "[sql], add channel error";
        logSqlError(q.lastError());
    } else {
        qDebug() << "[sql], add channel ok:" << title;
    }
}

// TODO: dialog ask what to do for existing one.
//  if tile==title, url==url, skip
//  if one of the comparsion is true, ask user to decide.
FindChannelResult SQLManager::findChannel(const QString &title, const QString &url) {
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
        QString ret_title, ret_url;
        if (q.next()) {
            ret_title = q.value("title").toString();
            ret_url = q.value("feedurl").toString();
            if (ret_title == title and ret_url == url) {
                return FIND;
            } else {
                return FIND_PARTLY;
            }
        }
        return NOTFIND;
    }
    return IN_VALID;
}

//TODO: check.
void SQLManager::addEpisode(int channelid, EpisodeData *ep) {
    assert(channelid != -1);
    QSqlQuery q;
    QString cmdstr = QString(
                         "insert into episodes (id, title, mediafileUrl, channelid, "
                         "description) values (%1, %2, %3, %4, %5)")
                         .arg(":id")
                         .arg(":title")
                         .arg(":url")
                         .arg(":channel")
                         .arg(":description");
    q.prepare(cmdstr);

    m_epID++;
    q.bindValue(":id", m_epID);
    q.bindValue(":title", ep->title);
    q.bindValue(":url", ep->url);
    q.bindValue(":channel", channelid);
    q.bindValue(":description", ep->description);
    // q.bindValue(":title", title);
    // q.bindValue(":url", url);

    auto ok = q.exec();
    checkReturn(ok, q, "add channel error");
}

void SQLManager::checkReturn(bool ok, QSqlQuery &q, const QString &msg, int line) {
    if (not ok) {
        if (line == -1)
            qDebug() << QString("[sql]: %1").arg(msg);
        else
            qDebug() << QString("[sql line%1]: %2").arg(line).arg(msg);
        logSqlError(q.lastError());
    }
}
