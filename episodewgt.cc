#include "episodewgt.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QDebug>

static int a = 0;

EpisodeWidget::EpisodeWidget(EpisodeData &data, QWidget *parent):QWidget(parent), id(a++),m_data(data) 
{

    auto *lay = new QVBoxLayout(this);
    lay->addWidget(new QLabel(QString("Episode: %1").arg(m_data.title)));
    lay->addWidget(new QTextEdit(QString("Detail: %1 ").arg(m_data.description)));
    this->setLayout(lay);
    // connect(this, &QWidget::mouse)
}

QString EpisodeWidget::msg() const 
{
    return QString("%1").arg(id);
}
