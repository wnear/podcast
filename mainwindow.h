#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <QMainWindow>

class Mainwindow : public QMainWindow
{
    public:
        Mainwindow (QWidget *parent = nullptr);
        ~Mainwindow () ;

    private:
        void setupMenu();
        class Private;
        Private *d;
    
};


#endif // !__MAIN_WINDOW_H
