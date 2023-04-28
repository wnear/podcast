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
    ~episodepropertymanager();
    void load();
    void save();
    void setVisiable(int index, bool visible);
    bool isVisible(int index) const;

    void getDialog();
    //iterator interface:


    //index,  title, visible
    std::vector<std::tuple<int, QString, bool>> m_data;
private:

};

#endif // EPISODEPROPERTYMANAGER_H
