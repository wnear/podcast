#include "utils.h"
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include "qstandardpaths.h"

namespace util {

QString int2hms(int secs) {
    int h, m, s;
    h = secs / 3600;
    m = (secs % 3600) / 60;
    s = (secs % 60);
    return QString("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

std::pair<float, int> size_recr(float size, int level = 0) {
    if (size < 1024)
        return std::make_pair(size, level);
    else
        return size_recr(size / 1024.0, level + 1);
    // return size > 1024 ? size_recr(size / 1024, level + 1) : std::make_pair(size,
    // level);
}

QString size_human(int size_byte) {
    auto [val, level] = size_recr(size_byte, 0);
    assert(level < 5);
    QStringList vol = {"Bytes", "KB", "MB", "GB", "TB", "YB"};
    return QString("%1 %2").arg(val, 0, 'f', 2).arg(vol[level]);
}

QString percent(float val) { return QString("[%1 %]").arg(val * 100, 0, 'f', 1); }

QString percentStr(int cur, int total) { return percent(cur * 1.0 / total); }


// bool file_is_newer(const QString &a, const QString &b) {
//     bool ret = QFileInfo(a).lastModified() > QFileInfo(b).lastModified();
//     qDebug() << "xml is " << (ret ? "newer" : "older");
//     return ret;
//     return true;
// }

QString ensureDirExist(const QString &parentdir, const QString &dirname) {
    auto path = QDir(parentdir).filePath(dirname);
    if (QFileInfo::exists(path) == false) {
        QDir(parentdir).mkdir(dirname);
    } else {
        // use empty string to as error.
        if (QFileInfo(path).isDir() == false) return "";
    }
    return path;
}

QString datapath() {
    QString app_datapath =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    return ensureDirExist(app_datapath, "podcast");
}

}  // namespace util
