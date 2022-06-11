#pragma once

#include <QString>

class Data
{
    public:
        static void init();
        static QString app_datapath();
        static QString podcast_datapath();
    private:
        static class DataPrivate *d;

};
