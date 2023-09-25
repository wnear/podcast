#include "episode_detail_wgt.h"
#include "utils.h"

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
#include <QTextBrowser>

#include <QStringListModel>

using namespace util;

class EpisodeDetailPrivate {
  public:
    QScrollArea *base;

    QTextBrowser detail;
    QPushButton play;
    QPushButton star;
    QPushButton ignore;

    QListView bookmark;
    QTextEdit notes;
};

EpisodeDetailWidget::EpisodeDetailWidget(QWidget *parent) : QWidget(parent) {
    d = new EpisodeDetailPrivate;

    d->base = new QScrollArea;
    auto lay = new QVBoxLayout;
    lay->addWidget(d->base);
    this->setLayout(lay);

    auto lay2 = new QVBoxLayout;
    lay2->addWidget(&d->detail);
    auto lay_btn = new QHBoxLayout;
    lay_btn->addWidget(&d->play);
    lay_btn->addWidget(&d->star);
    lay_btn->addWidget(&d->ignore);
    lay2->addLayout(lay_btn);

    auto sp = new QSplitter;
    sp->addWidget(&d->bookmark);
    sp->addWidget(&d->notes);
    lay2->addWidget(sp);

    auto model = new QStringListModel;
    model->setStringList({"hello", "world"});

    d->bookmark.setModel(model);

    d->base->setLayout(lay2);

    d->detail.setOpenLinks(false);
    d->detail.setOpenExternalLinks(false);
    connect(&d->detail, &QTextBrowser::anchorClicked, this, [this](const QUrl &url) {
        if (url.scheme() == "positions") {
            //TODO:
            qDebug()<<"need to jump to "<<url.fragment();
            emit requestEpisodePosition(m_cur_episode, url.fragment());
        } else {
            emit requestOpenLink(url);
        }
    });
}

EpisodeDetailWidget::~EpisodeDetailWidget() { delete d; }
void EpisodeDetailWidget::setData(EpisodeData *ep) {
    m_cur_episode = ep;

    // d->detail.setText(m_cur_episode->description);
    // d->detail.textCursor();

    d->detail.clear();
    auto cursor = d->detail.textCursor();
    cursor.beginEditBlock();

    QTextCharFormat normalFormat = cursor.charFormat();
    normalFormat.setFontPointSize(10);

    QTextCharFormat titleFormat = normalFormat;
    titleFormat.setFontWeight(QFont::Bold);
    titleFormat.setFontPointSize(normalFormat.fontPointSize() + 2);
    // highlightedFormat.setBackground(Qt::yellow);

    cursor.insertText(m_cur_episode->title, titleFormat);
    cursor.insertText("\n");
    cursor.insertText(QString("duration: %1| size: %2")
                          .arg(int2hms(m_cur_episode->duration))
                          .arg(size_human(m_cur_episode->filesize)),
                      normalFormat);
    cursor.insertText("\n");
    auto info = m_cur_episode->description;
    make_clickable_timestamp(info);
    cursor.insertHtml(info);
    cursor.endEditBlock();
}
