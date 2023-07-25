#include "sqlmanager.h"
#include "utils.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSqlQuery>
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

    assert(0);
    m_channelID = 0;
    m_epID = 0;
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
void SQLManager::addChannel(const QString &title, const QString &url) {
    QSqlQuery q;
    QString cmdstr =
        QString("insert into localchannel (id, title, feedurl) values (%1, %2, %3)")
            .arg(":id")
            .arg(":title")
            .arg(":url");
    q.prepare(cmdstr);

    q.bindValue(":id", m_channelID);
    q.bindValue(":title", title);
    q.bindValue(":url", url);
    m_channelID++;

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
bool SQLManager::findChannel(const QString &title, const QString &url) {
    // res = false;
    // res |= find with title,
    // res |= find with url,
    return false;
}
void SQLManager::addEpisode(const QString &feedurl) {
    assert(0);
    QSqlQuery q;
    QString cmdstr =
        QString("insert into episodes (id, title, feedurl) values (%1, %2, %3)")
            .arg(":id")
            .arg(":title")
            .arg(":url");
    q.prepare(cmdstr);

    q.bindValue(":id", m_epID);
    // q.bindValue(":title", title);
    // q.bindValue(":url", url);
    m_epID++;

    auto ok = q.exec();

    if (!ok) {
        qDebug() << "[sql], add channel error";
        logSqlError(q.lastError());
    } else {
        qDebug() << "[sql], add channel ok:" ;
    }
}

