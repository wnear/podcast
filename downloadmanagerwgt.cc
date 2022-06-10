
#include "downloadmanagerwgt.h"
#include "downloadmanager.h"
#include <QTextEdit>
#include <QVBoxLayout>


DownloadManagerWidget:: DownloadManagerWidget(QWidget *parent )
:QDialog(parent)
{
    this->setMinimumHeight(800);
    this->setMinimumHeight(800);
    auto lay = new QVBoxLayout(this);
    auto edit = new QTextEdit(this);
    edit->setText(DownloadManager::instance()->toString());
    lay->addWidget(edit);
    this->setLayout(lay);
}
