#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <QMainWindow>

class Mainwindow : public QMainWindow {
    Q_OBJECT

  public:
    Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();
    void closeEvent(QCloseEvent *event) override;

  private:
    void setupMenu();
    void setupToolbar();
    class Private;
    Private *d;
};

#endif  // !__MAIN_WINDOW_H
