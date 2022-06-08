#include "episodewgt.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

static int a = 0;

EpisodeWidget::EpisodeWidget(QWidget *parent):QWidget(parent), id(a++)
{
    auto *lay = new QHBoxLayout(this);
    lay->addWidget(new QPushButton("add"));
    lay->addWidget(new QPushButton("delete"));
    lay->addWidget(new QLabel(QString("label of %1").arg(id)));
    this->setLayout(lay);
    // connect(this, &QWidget::mouse)
}

QString EpisodeWidget::msg() const 
{
    return QString("%1").arg(id);
}
