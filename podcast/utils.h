#pragma once
#include <QString>

namespace util {

QString int2hms(int secs);

QString size_human(int size_byte);
QString percent(float val);
QString percentStr(int cur, int total);

bool file_is_newer(const QString &a, const QString &b);

};  // namespace util
