#include "trem.h"
#include "/home/jordao/dev/semaforo.h"

Trem::Trem(int id, int x, int y)
{
    this->id = id;
    this->x = x;
    this->y = y;
    velocidade = 250;
    enable = true;
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

void Trem::start()
{
    threadTrem = std::thread(&Trem::run,this);
}

void Trem::run()
{
    while(true){
        switch(id){
        case 1:
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
            break;
        case 2:
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
            break;
        case 3:
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
            break;
        case 4:
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
            break;
        case 5:
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
            break;
        case 6:
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
            break;
        default:
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(velocidade));
    }
}

