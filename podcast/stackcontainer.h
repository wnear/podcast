#pragma once

#include <QStackedWidget>
#include <QMap>
#include "stackwgt.h"
#include "stackdata.h"

class StackContainer : public QStackedWidget {
    Q_OBJECT

  public:
    static StackContainer *instance(QWidget *parent = nullptr);

    void register_element(StackWidget *wgt, StackData *data);
    void init();
    void push(StackData *data);
    void back();
  signals:
    void moduleChanged(const QString &name);

  private:
    StackContainer(QWidget *parent = nullptr);
    QList<StackData *> m_history;
    QMap<QString, StackWidget *> m_registerd;
};
