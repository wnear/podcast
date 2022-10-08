#pragma once

#include <QString>

class Data
{
    public:
        static void init();
        static QString app_datapath();
        static QString podcast_datapath();
        static QString podcast_datapath(const QString &title);

      private:
        static class DataPrivate *d;

};
