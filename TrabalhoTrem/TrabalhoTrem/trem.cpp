#include "trem.h"
#include "/home/jordao/dev/semaforo.h"
#include <iostream>

Trem::Trem(int id, int x, int y, Semaforo **sem_param)
{
    this->id = id;
    this->x = x;
    this->y = y;
    velocidade = 250;
    enable = true;

    sem[0]=sem_param[0];
    sem[1]=sem_param[1];
    sem[2]=sem_param[2];
    sem[3]=sem_param[3];
    sem[4]=sem_param[4];
    sem[5]=sem_param[5];
    sem[6]=sem_param[6];
    sem[7]=sem_param[7];

}

Trem::~Trem()
{
    threadTrem.join();
}

void Trem::setVelocidade(int velocidade)
{
    this->velocidade = velocidade;
}

void Trem::setEnable(bool enable)
{
    this->enable = enable;
}

bool Trem::getEnable()
{
    return this->enable;
}

void Trem::start()
{
    threadTrem = std::thread(&Trem::run,this);
}

void Trem::run()
{
    while(true){
        switch(id){
        case 1:
            //if (enable)
            //{
                emit updateGUI(id,x,y);
                if (y == 220 && x == 140)
                {
                    sem[0]->P();
                    sem[1]->P();
                }
                if (enable)
                {
                    emit updateGUI(id,x,y);
                    if (y == 220 && x < 350)
                        x+=10;
                    else if (x == 350 && y < 320)
                        y+=10;
                    else if (x > 130 && y == 320)
                        x-=10;
                    else
                        y-=10;
                }
                if (y == 250 && x == 350)
                    sem[1]->V();
                if (y == 320 && x == 320)
                    sem[0]->V();
            //}
            break;
        case 2:
            //if (enable)
            //{
                emit updateGUI(id,x,y);
                if (y == 320 && x == 380)
                {
                    sem[2]->P();
                    sem[0]->P();
                }
                if (enable)
                {
                    emit updateGUI(id,x,y);
                    if (y == 220 && x < 560)
                        x+=10;
                    else if (x == 560 && y < 320)
                        y+=10;
                    else if (x > 350 && y == 320)
                        x-=10;
                    else
                        y-=10;
                }
                if (y == 220 && x == 380)
                    sem[0]->V();
                if (y == 220 && x == 540)
                    sem[2]->V();
            //}
            break;
        case 3:
            //if (enable)
            //{
                emit updateGUI(id,x,y);
                if (y == 150 && x == 510)
                {
                    sem[3]->P();
                    sem[4]->P();
                    sem[5]->P();
                }
                if (y == 190 && x == 170)
                {
                    sem[2]->P();
                    sem[1]->P();
                }
                if (enable)
                {
                    emit updateGUI(id,x,y);
                    if (y == 220 && x < 510)
                        x+=10;
                    else if (x == 170 && y < 220)
                        y+=10;
                    else if (x > 170 && y == 120)
                        x-=10;
                    else
                        y-=10;
                }
                if (y == 120 && x == 390)
                    sem[5]->V();
                if (y == 120 && x == 230)
                    sem[4]->V();
                if (y == 150 && x == 170)
                    sem[3]->V();
                if (y == 220 && x == 380)
                    sem[1]->V();
                if (y == 190 && x == 510)
                    sem[2]->V();
            //}
            break;
        case 4:
            //if (enable)
            //{
                emit updateGUI(id,x,y);
                if (y == 120 && x == 540)
                {
                    sem[6]->P();
                    sem[5]->P();
                }
                if (enable)
                {
                    emit updateGUI(id,x,y);
                    if (y == 20 && x < 560)
                        x+=10;
                    else if (x == 560 && y < 120)
                        y+=10;
                    else if (x > 420 && y == 120)
                        x-=10;
                    else
                        y-=10;
                }
                if (y == 90 && x == 420)
                    sem[5]->V();
                if (y == 20 && x == 450)
                    sem[6]->V();
            //}
            break;
        case 5:
            //if (enable)
            //{
                emit updateGUI(id,x,y);
                if (y == 20 && x == 290)
                {
                    sem[6]->P();
                    sem[4]->P();
                    sem[7]->P();
                }
                if (enable)
                {
                    emit updateGUI(id,x,y);
                    if (y == 120 && x < 420)
                        x+=10;
                    else if (x == 260 && y < 120)
                        y+=10;
                    else if (x > 260 && y == 20)
                        x-=10;
                    else
                        y-=10;
                }
                if (y == 120 && x == 290)
                    sem[7]->V();
                if (y == 90 && x == 420)
                    sem[4]->V();
                if (y == 20 && x == 390)
                    sem[6]->V();
            //}
            break;
        case 6:
            //if (enable)
            //{
                //std::cout<<(sem[0]->getContador())<<endl;
                emit updateGUI(id,x,y);
                if (y == 20 && x == 230)
                {
                    sem[3]->P();
                    sem[7]->P();
                }
                if (enable)
                {
                    emit updateGUI(id,x,y);
                    if (y == 20 && x < 260)
                        x+=10;
                    else if (x == 260 && y < 120)
                        y+=10;
                    else if (x > 120 && y == 120)
                        x-=10;
                    else
                        y-=10;
                }
                if (y == 120 && x == 230)
                    sem[7]->V();
                if (y == 120 && x == 140)
                    sem[3]->V();
            //}
            break;
        default:
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(velocidade));
    }
}

