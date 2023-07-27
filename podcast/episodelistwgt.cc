#include "episodelistwgt.h"
#include "podcastchannel.h"
#include "episodewidgetitem.h"
#include "log.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>

struct EpisodeListWidgetPrivate {
    QScrollArea *scrollArea;
    QVBoxLayout *listLay;

    QVBoxLayout *lay;
};

EpisodeListWidget::EpisodeListWidget(QWidget *parent) : QWidget(parent) {
    d = new EpisodeListWidgetPrivate;
    d->scrollArea = new QScrollArea(this);
    d->scrollArea->setWidget(new QWidget(this));
    // d->scrollArea->setFrameShape(QFrame::NoFrame);  //枠線をなくす
    //  d->list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//表示しない
    d->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // 常に表示
    d->scrollArea->setWidgetResizable(true);  // 先にこれを設定する。

    scrollWidget()->setLayout(new QVBoxLayout);

    d->lay = new QVBoxLayout(this);
    d->lay->addWidget(d->scrollArea, 1);
    this->setLayout(d->lay);
}

QWidget *EpisodeListWidget::scrollWidget() { return (d->scrollArea->widget()); }

void EpisodeListWidget::refresh() { this->setPod(cur); }

QString EpisodeListWidget::current() const { return cur == nullptr ? "" : cur->title; }

void EpisodeListWidget::setPod(PodcastChannel *pod) {
    cur = pod;
    auto childs = this->findChildren<EpisodeWidgetItem *>();
    scrollWidget()->findChild<QWidget *>();
    for (auto i : childs) i->deleteLater();

    int count = pod->episodes.count();
    count = std::min(10, count);
    if (count == 0) return;

    binfo("episodelistwget draw...");
    for (int i = 0; i < count; i++) {
        scrollWidget()->layout()->addWidget(
            new EpisodeWidgetItem((EpisodeData *)(pod->episodes[i]), scrollWidget()));
    }
}
