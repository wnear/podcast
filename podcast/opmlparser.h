#pragma once

#include <QString>

using channel_item_t = std::pair<QString, QString>;

class OpmlParser {
  public:
    // using elems_t = QList<channel_item_t>;
    OpmlParser(const QString &filename) : m_filename(filename) {}

    QList<channel_item_t> parse();

  private:
    QString m_filename;
};
