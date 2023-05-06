#pragma once

#include <QString>

class OpmlParser {
  public:
    using elem_t = std::pair<QString, QString>;
    using elems_t = QList<elem_t>;
    OpmlParser(const QString &filename) : m_filename(filename) {}

    elems_t parse();

  private:
    QString m_filename;
};
