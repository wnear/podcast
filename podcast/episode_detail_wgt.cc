#include "episode_detail_wgt.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QPushButton>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QListView>

#include <QStringListModel>

class EpisodeDetailPrivate {
public:
    QScrollArea *base;

    QLabel detail;
    QPushButton play;
    QPushButton star;
    QPushButton ignore;

    QListView bookmark;
    QTextEdit notes;
};

EpisodeDetail::EpisodeDetail(QWidget *parent)
:QWidget(parent)
{
    d = new EpisodeDetailPrivate;

    d->base = new QScrollArea;
    auto lay = new QVBoxLayout;
    lay->addWidget(d->base);
    this->setLayout(lay);

    auto lay2 = new QVBoxLayout;
    lay2->addWidget(& d->detail);
    auto lay_btn = new QHBoxLayout;
    lay_btn ->addWidget(& d->play);
    lay_btn ->addWidget(& d->star);
    lay_btn ->addWidget(& d->ignore);
    lay2->addLayout(lay_btn);

    auto sp = new QSplitter;
    sp->addWidget(& d->bookmark);
    sp->addWidget(& d->notes);
    lay2->addWidget(sp);

    auto model = new QStringListModel;
    model->setStringList({"hello", "world"});

    d->bookmark.setModel(model);

    d->base->setLayout(lay2);
}


EpisodeDetail::~EpisodeDetail()
{
    delete d;
}
void EpisodeDetail::setData(EpisodeData *data)
{
    m_data = data;
    d->detail.setText(m_data->description);
}
