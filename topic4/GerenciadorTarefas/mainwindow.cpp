#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QApplication>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<ctime>
#include<utility>
#include<vector>
#include<sstream>
#include<dirent.h>
#include<fstream>
#include<cstring>
#include<algorithm>
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
using namespace std;

struct sysinfo memInfo;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->webViewProcessos->load(QUrl("file:///home/jordao/operationalsystems/teste/index.html"));

    inicializarGraficoCPU();
    inicializarGraficoMemoria();
    inicializarGraficoCarga();
    inicializarGraficoTempoDescarga();
    connect(ui->pushButtonKill,SIGNAL(clicked(bool)),this,SLOT(matar()));
    connect(ui->pushButtonAtualizar,SIGNAL(clicked(bool)),this,SLOT(atualizar()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inicializarGraficoCPU()
{
    //Adicionar CPU 1 - Azul
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));

    //Adicionar CPU 2 - Vermelha
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));

    //Adicionar circulo azul seguidor da CPU 1
    ui->customPlot->addGraph(); // blue dot
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Adicionar circulo vermelho seguidor da CPU 2
    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Configuração do eixo X
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setTickLabelRotation(90);
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    //Configuração da Legenda
    ui->customPlot->graph(0)->setName("CPU 1");
    ui->customPlot->graph(1)->setName("CPU 2");
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->removeItem(3); //remover a legenda do circulo seguidor
    ui->customPlot->legend->removeItem(2); //remover a legenda do circulo seguidor
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->legend->setRowSpacing(-10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    //MODIFICAR ESSA FUNCIONALIDADE PELA EXECUÇÃO DE UMA THREAD

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(atualizarGraficoCPU()));
    dataTimer.start(500); // Interval 0 means to refresh as fast as possible
}

void MainWindow::inicializarGraficoMemoria()
{
    //Adicionar Memória Principal - Azul
    ui->customPlot2->addGraph();
    ui->customPlot2->graph(0)->setPen(QPen(Qt::blue));

    //Adicionar Swap - Vermelha
    ui->customPlot2->addGraph();
    ui->customPlot2->graph(1)->setPen(QPen(Qt::red));

    //Adicionar circulo azul seguidor da memória principal
    ui->customPlot2->addGraph(); // blue dot
    ui->customPlot2->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot2->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot2->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Adicionar circulo vermelho seguidor do swap
    ui->customPlot2->addGraph(); // red dot
    ui->customPlot2->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot2->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot2->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Configuração do eixo X
    ui->customPlot2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot2->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot2->xAxis->setTickLabelRotation(90);
    ui->customPlot2->xAxis->setAutoTickStep(false);
    ui->customPlot2->xAxis->setTickStep(2);
    ui->customPlot2->axisRect()->setupFullAxesBox();

    //Configuração da Legenda
    ui->customPlot2->graph(0)->setName("RAM");
    ui->customPlot2->graph(1)->setName("Swap");
    ui->customPlot2->legend->setVisible(true);
    ui->customPlot2->legend->removeItem(3); //remover a legenda do circulo seguidor
    ui->customPlot2->legend->removeItem(2); //remover a legenda do circulo seguidor
    ui->customPlot2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->customPlot2->legend->setFont(QFont("Helvetica",9));
    ui->customPlot2->legend->setRowSpacing(-10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot2->yAxis2, SLOT(setRange(QCPRange)));



    //MODIFICAR ESSA FUNCIONALIDADE PELA EXECUÇÃO DE UMA THREAD

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&memoriaTimer, SIGNAL(timeout()), this, SLOT(atualizarGraficoMemoria()));
    memoriaTimer.start(0); // Interval 0 means to refresh as fast as possible
}


void MainWindow::inicializarGraficoCarga()
{
    //Adicionar Carga - Azul
    ui->customPlot3->addGraph();
    ui->customPlot3->graph(0)->setPen(QPen(Qt::blue));

    //Adicionar circulo azul seguidor da Carga
    ui->customPlot3->addGraph(); // blue dot
    ui->customPlot3->graph(1)->setPen(QPen(Qt::blue));
    ui->customPlot3->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot3->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Configuração do eixo X
    ui->customPlot3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot3->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot3->xAxis->setTickLabelRotation(90);
    ui->customPlot3->xAxis->setAutoTickStep(false);
    ui->customPlot3->xAxis->setTickStep(2);
    ui->customPlot3->axisRect()->setupFullAxesBox();

    //Configuração da Legenda
    ui->customPlot3->graph(0)->setName("Carga (%)");
    ui->customPlot3->legend->setVisible(true);
    ui->customPlot3->legend->removeItem(1); //remover a legenda do circulo seguidor
    ui->customPlot3->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->customPlot3->legend->setFont(QFont("Helvetica",9));
    ui->customPlot3->legend->setRowSpacing(-10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot3->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot3->yAxis2, SLOT(setRange(QCPRange)));

    //MODIFICAR ESSA FUNCIONALIDADE PELA EXECUÇÃO DE UMA THREAD

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&cargaTimer, SIGNAL(timeout()), this, SLOT(atualizarGraficoCarga()));
    cargaTimer.start(500); // Interval 0 means to refresh as fast as possible
}


void MainWindow::inicializarGraficoTempoDescarga()
{
    //Adicionar Tempo Descarga - Vermelho
    ui->customPlot4->addGraph();
    ui->customPlot4->graph(0)->setPen(QPen(Qt::red));

    //Adicionar circulo vermelho seguidor da Carga
    ui->customPlot4->addGraph(); // red dot
    ui->customPlot4->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot4->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot4->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Configuração do eixo X
    ui->customPlot4->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot4->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot4->xAxis->setTickLabelRotation(90);
    ui->customPlot4->xAxis->setAutoTickStep(false);
    ui->customPlot4->xAxis->setTickStep(2);
    ui->customPlot4->axisRect()->setupFullAxesBox();

    //Configuração da Legenda
    ui->customPlot4->graph(0)->setName("Tempo de Descarga");
    ui->customPlot4->legend->setVisible(true);
    ui->customPlot4->legend->removeItem(1); //remover a legenda do circulo seguidor
    ui->customPlot4->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->customPlot4->legend->setFont(QFont("Helvetica",9));
    ui->customPlot4->legend->setRowSpacing(-10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot4->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot4->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot4->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot4->yAxis2, SLOT(setRange(QCPRange)));

    //MODIFICAR ESSA FUNCIONALIDADE PELA EXECUÇÃO DE UMA THREAD

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&tempoDescargaTimer, SIGNAL(timeout()), this, SLOT(atualizarGraficoTempoDescarga()));
    tempoDescargaTimer.start(500); // Interval 0 means to refresh as fast as possible
}

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;


    void init(){
        FILE* file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
            &lastTotalSys, &lastTotalIdle);
        fclose(file);
    }


    double getCurrentValue(){
        double percent;
        FILE* file;
        unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;


        file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
            &totalSys, &totalIdle);
        fclose(file);


        if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
            totalSys < lastTotalSys || totalIdle < lastTotalIdle){
            //Overflow detection. Just skip this value.
            percent = -1.0;
        }
        else{
            total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
                (totalSys - lastTotalSys);
            percent = total;
            total += (totalIdle - lastTotalIdle);
            percent /= total;
            percent *= 100;
        }


        lastTotalUser = totalUser;
        lastTotalUserLow = totalUserLow;
        lastTotalSys = totalSys;
        lastTotalIdle = totalIdle;


        return percent;
    }

void MainWindow::atualizarGraficoCPU()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double value0 = getCurrentValue();
    double value1 = qCos(key);

    // add data to lines:
      ui->customPlot->graph(0)->addData(key, value0);
      ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->customPlot->graph(2)->clearData();
      ui->customPlot->graph(2)->addData(key, value0);
      ui->customPlot->graph(3)->clearData();
      ui->customPlot->graph(3)->addData(key, value1);

      ui->customPlot->graph(0)->rescaleValueAxis();
      ui->customPlot->graph(1)->rescaleValueAxis(true);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25,60, Qt::AlignRight);
    ui->customPlot->replot();

}

void MainWindow::atualizarGraficoMemoria()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    sysinfo (&memInfo);

    double swap = ((double)(memInfo.totalswap - memInfo.freeswap)) /  ((double)memInfo.totalswap);

    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;

    long long totalPhysMem = memInfo.totalram;
    //Multiply in next statement to avoid int overflow on right hand side...
    totalPhysMem *= memInfo.mem_unit;

   // http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

    double value0 = (double)physMemUsed/(double)totalPhysMem;
    //cout<<value0<<endl;
    double value1 = swap;

    // add data to lines:
      ui->customPlot2->graph(0)->addData(key, value0);
      ui->customPlot2->graph(1)->addData(key, value1);

      // set data of dots:
      ui->customPlot2->graph(2)->clearData();
      ui->customPlot2->graph(2)->addData(key, value0);
      ui->customPlot2->graph(3)->clearData();
      ui->customPlot2->graph(3)->addData(key, value1);

      ui->customPlot2->graph(0)->rescaleValueAxis();
      ui->customPlot2->graph(1)->rescaleValueAxis(true);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot2->xAxis->setRange(key+0.25,60, Qt::AlignRight);
    ui->customPlot2->replot();

}

void MainWindow::atualizarGraficoCarga()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double value0 = qSin(key);

    // add data to lines:
    ui->customPlot3->graph(0)->addData(key, value0);

    // set data of dots:
    ui->customPlot3->graph(1)->clearData();
    ui->customPlot3->graph(1)->addData(key, value0);

    ui->customPlot3->graph(0)->rescaleValueAxis();

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot3->xAxis->setRange(key+0.25,60, Qt::AlignRight);
    ui->customPlot3->replot();

}


void MainWindow::atualizarGraficoTempoDescarga()
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double value0 = qSin(key);

    // add data to lines:
    ui->customPlot4->graph(0)->addData(key, value0);

    // set data of dots:
    ui->customPlot4->graph(1)->clearData();
    ui->customPlot4->graph(1)->addData(key, value0);

    ui->customPlot4->graph(0)->rescaleValueAxis();

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot4->xAxis->setRange(key+0.25,60, Qt::AlignRight);
    ui->customPlot4->replot();
}

void MainWindow::matar()
{
    int a;
    a = ui->lineEditPID->text().toInt();

    stringstream ss;
    ss << "kill " << a;

    system(ss.str().c_str());
}


/*******************/


ofstream wjson;

bool wis_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

vector<string> wlistFile(){
    vector<string> retorno;

    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir("/proc/") ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            //cout << entry->d_name << "\n";
            //if(entry->d_name[0]=='f' &&  entry->d_name[1]=='i' &&  entry->d_name[2]=='l' &&  entry->d_name[3]=='h')
            if ( wis_number(entry->d_name))
            retorno.push_back(entry->d_name);
        }
        closedir(pDIR);
    }
    return retorno;
}

int wcpu()
{
    ifstream myfile;
    myfile.open("/proc/stat");

    string teste;
    int cpu0, cpu1, sum;
    float average;

    myfile >> teste;
    myfile >> cpu0;
    sleep(10);
    myfile >> teste;
    myfile >> cpu1;

    sum = cpu0 + cpu1;
    average = (sum/2);

    return average;
}

vector<int> wlistChildren(int parent)
{
    stringstream ss;
    ss << "ps --ppid " << parent << " > tmp";

    //cout << ss.str();

    vector<int> retorno;

    system(ss.str().c_str());

    ifstream myfile;
    myfile.open("tmp");

    string teste;
    int cpid;

    myfile >> teste;

    while(true)
    {
        myfile >> teste;
        myfile >> teste;
        myfile >> teste;
        myfile >> cpid;
        if( myfile.eof() ) break;
        retorno.push_back(cpid);
    }
    return retorno;

}

string wgetPName(int pid)
{
    stringstream ss;
    ss << "ps -p " << pid << " -o comm= > tmp2";

    system(ss.str().c_str());

    ifstream myfile;
    myfile.open("tmp2");

    string retorno;

    myfile >> retorno;

    return retorno;
}


int wmemorybypid(int pid)
{
    stringstream ss;
    ss << "/proc/" << pid << "/statm";

    ifstream myfile;
    myfile.open(ss.str().c_str());

    int retorno;

    myfile >> retorno;

    return retorno;
}

void wrecursiveJSon(int processo,int nivel)
{
    vector<int> children = wlistChildren(processo);

    wjson<<string(nivel*4,' ')<<"{"<<endl;
    wjson<<string(nivel*4,' ')<<"\"name\" : \""<< wgetPName(processo) <<"\","<<endl;
    wjson<<string(nivel*4,' ')<<"\"children\":"<<endl;
    wjson<<string(nivel*4,' ')<<"["<<endl;
                                                                             // colocar memoria usada pelo processo
    wjson<<string(nivel*4,' ')<<"{\"name\": \""<<wgetPName(processo)<<"\", \"size\":  "<<wmemorybypid(processo)/1000<<", \"pid\": "<<processo<<"}"<<endl;

    for(auto x:children)
    {
        wjson<<","<<endl;
        wrecursiveJSon(x,nivel+1);
    }

    wjson<<string(nivel*4,' ')<<"]"<<endl;


    wjson<<string(nivel*4,' ')<<"}";

}



void MainWindow::atualizar()
{
    wjson.open ("/home/jordao/operationalsystems/teste/processos.json");
    wrecursiveJSon(1,0);
    wjson.close();

    //QString fn = "/home/jordao/operationalsystems/teste/index.html";
    //QFileInfo fi(fn);
    //cout<<"oi "<< fi.canonicalFilePath().toStdString() <<endl;

    ui->webViewProcessos->load(QUrl("file:///home/jordao/operationalsystems/teste/index.html"));
    ui->webViewProcessos->reload();
    ui->webViewProcessos->repaint();
    ui->webViewProcessos->update();
}
