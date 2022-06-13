
#include "utils.h"

QString int2hms(int secs){
    int h,m,s;
    h = secs/3600;
    m = (secs%3600)/60;
    s = (secs%60);
    return QString("%1:%2:%3")
        .arg(h, 2,10,QChar('0'))
        .arg(m, 2,10,QChar('0'))
        .arg(s, 2,10,QChar('0'));
}
QString size_human(int size_byte) { return QString("%1 MB").arg(size_byte * 1.0 / 1024 / 1024, 0, 'f', 2); }
QString percent(float val) { return QString("[%1 %]").arg(val * 100, 0, 'f', 1); }
QString percentStr(int cur, int total) { return percent(cur * 1.0 / total); }
