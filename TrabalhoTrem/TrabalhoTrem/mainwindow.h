#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "trem.h"
#include "/home/jordao/dev/semaforo.h"
#include <QMainWindow>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateInterface(int,int,int);

private:
    Ui::MainWindow *ui;
    Trem *trem1;
    Trem *trem2;
    Trem *trem3;
    Trem *trem4;
    Trem *trem5;
    Trem *trem6;
    //Semaforo *sem1, *sem2, *sem3, *sem4, *sem5, *sem6, *sem7, *sem8;
};

#endif // MAINWINDOW_H
