#ifndef EPISODEPROPERTYMANAGER_H
#define EPISODEPROPERTYMANAGER_H
#include <vector>
#include <tuple>
#include <QString>



/*
 * all properties.
 * visible properties.
 * control.
 *
 * Design note:
 * - THIS class should be separated from the episode detail, only
 * to help design the tree-view.
 * Then the underlying model-data should implement the
 * detail as much as possible.
 *
 * - It act as a tree-view header menu control interface helper.
 *
 * - The visible choices should be stored.
 *
 */
enum TreeColumn {
    TITLE = 0,
    UPDATETIME,
    SIZE,
    DURATION,
    URL,
    LASTVIEW,
    LOCATION,
    PLAYTIME,
    CNT_SENTEL
};


class episodepropertymanager
{
public:
    episodepropertymanager();
private:
    //index,  title, visible
    std::vector<std::tuple<int, QString, bool>> m_data;
};

#endif // EPISODEPROPERTYMANAGER_H
