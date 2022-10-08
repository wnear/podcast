#include "stackcontainer.h"

StackContainer::StackContainer(QWidget *parent)
                            :QStackedWidget(parent)
{
}

StackContainer *StackContainer::instance(QWidget *parent) {
    static StackContainer *ins{nullptr};
    if (ins == nullptr) {
        ins = new StackContainer(parent);
    } else {
        if (parent != nullptr) assert(0);
    }

    return ins;
}
void StackContainer::register_element(StackWidget *wgt, StackData *data)
{
    this->addWidget(wgt);
    m_registerd.insert(data->metaObject()->className(), wgt);
}
void StackContainer::push(StackData *data)
{
    m_history.push_back(data);
    this->setCurrentWidget(m_registerd[data->metaObject()->className()]);
}


void StackContainer::back()
{

}
