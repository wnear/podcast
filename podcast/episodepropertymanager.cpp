#include "episodepropertymanager.h"
#include <QDialog>

using namespace std;

episodepropertymanager::episodepropertymanager()
{
    m_data.push_back(std::make_tuple(0, "TITLE", false));

    this->load();
}
episodepropertymanager::~episodepropertymanager()
{
    this->save();
}
/*
 * index,  title, visible
   std::vector<std::tuple<int, QString, bool>> m_data;
*/
void episodepropertymanager::load()
{
    //TODO:
}

void episodepropertymanager::save()
{
    //TODO:
}

void episodepropertymanager::setVisiable(int index, bool visible)
{
    auto [idx, title, status] = m_data[index];
    m_data[index] = std::make_tuple(idx, title, visible);
}

bool episodepropertymanager::isVisible(int index) const
{
    auto [idx, title, status] = m_data[index];
    return status;
}

void episodepropertymanager::getDialog()
{
    auto dialog = new QDialog();
    //TODO:
}

