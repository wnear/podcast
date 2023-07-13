#pragma once
#include <QString>


namespace util {

QString int2hms(int secs);
void make_clickable_timestamp(QString &orig, const QString &scheme = "positions");

QString size_human(int size_byte);
QString percent(float val);
QString percentStr(int cur, int total);

// Generic file compare, T can be QString or QFile, or string.
template <typename T>
bool file_is_newer(const T &a, const T &b){
    bool ret = QFileInfo(a).lastModified() > QFileInfo(b).lastModified();
    // qDebug() << QString("xml is") << QString(ret ? "newer" : "older");
    return ret;
}
QString ensureDirExist(const QString &parentdir, const QString &dirname);



QString datapath();

};  // namespace util
