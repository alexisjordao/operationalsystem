#ifndef TREM_H
#define TREM_H

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <QObject>
#include <thread>
#include <chrono>
#include "/home/jordao/dev/semaforo.h"
using namespace std;

class Trem : public QObject
{
    Q_OBJECT
public:
    Trem(int,int,int,Semaforo**);
    ~Trem();
    void start();
    void run();
    void setVelocidade(int);
    void setEnable(bool);
    bool getEnable();

signals:
    void updateGUI(int,int,int);

private:
   std::thread threadTrem;
   int id;
   int x;
   int y;
   int velocidade;
   bool enable;
   //Semaforo *sem1, *sem2, *sem3, *sem4, *sem5, *sem6, *sem7, *sem8;
   Semaforo *sem[8];
};

#endif // TREM_H



