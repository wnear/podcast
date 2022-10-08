#pragma once

#include <QFrame>

class StackWidget: public QFrame
{
    public:
        StackWidget(QWidget*parent = nullptr){}
        virtual int id() const = 0;
};

