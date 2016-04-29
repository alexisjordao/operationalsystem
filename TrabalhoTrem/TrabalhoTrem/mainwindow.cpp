#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <unistd.h>

#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket

#define MAXMSG 1024
#define PORTNUM 4325

Trem *gtrem1;
Trem *gtrem2;
Trem *gtrem3;
Trem *gtrem4;
Trem *gtrem5;
Trem *gtrem6;

void toggle_all()
{
    bool novo = !(gtrem1->getEnable() || gtrem2->getEnable() || gtrem3->getEnable() || gtrem4->getEnable() || gtrem5->getEnable() || gtrem6->getEnable() );

    gtrem1->setEnable(novo);
    gtrem2->setEnable(novo);
    gtrem3->setEnable(novo);
    gtrem4->setEnable(novo);
    gtrem5->setEnable(novo);
    gtrem6->setEnable(novo);
}

void servidor()
{
    struct sockaddr_in endereco;
    int socketId;

    //variáveis relacionadas com as conexões clientes
    struct sockaddr_in enderecoCliente;
    socklen_t tamanhoEnderecoCliente = sizeof(struct sockaddr);
    int conexaoClienteId;

    //Permitir inserir o caractere de fim de msg \0
    char *msg = new char[100];
    int first,second;
    int byteslidos;

    /*
     * Configurações do endereço
    */
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    //endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_addr.s_addr = inet_addr("127.0.0.1");

    /*
     * Criando o Socket
     *
     * PARAM1: AF_INET ou AF_INET6 (IPV4 ou IPV6)
     * PARAM2: SOCK_STREAM ou SOCK_DGRAM
     * PARAM3: protocolo (IP, UDP, TCP, etc). Valor 0 escolhe automaticamente
    */
    socketId = socket(AF_INET, SOCK_STREAM, 0);

    //Verificar erros
    if (socketId == -1)
    {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    //Conectando o socket a uma porta. Executado apenas no lado servidor
    if ( bind (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
    {
        printf("Falha ao executar bind()\n");
        exit(EXIT_FAILURE);
    }

    //Habilitando o servidor a receber conexoes do cliente
    if ( listen( socketId, 10 ) == -1)
    {
        printf("Falha ao executar listen()\n");
        exit(EXIT_FAILURE);
    }
    cout<<"Terminou o setup do servidor"<<endl;

    //servidor ficar em um loop infinito
    while(1)
    {

        cout<<"Servidor: esperando conexões clientes\n"<<endl;

        //Servidor fica bloqueado esperando uma conexão do cliente
        conexaoClienteId = accept( socketId,(struct sockaddr *) &enderecoCliente,&tamanhoEnderecoCliente );

        printf("Servidor: recebeu conexão de %s\n", inet_ntoa(enderecoCliente.sin_addr));

        //Verificando erros
        if ( conexaoClienteId == -1)
        {
            printf("Falha ao executar accept()");
            exit(EXIT_FAILURE);
        }

        //receber uma msg do cliente
        printf("Servidor vai ficar esperando uma mensagem\n");

        byteslidos = 1;
        int finitestate=0;

        while(byteslidos)
        {
            byteslidos = recv(conexaoClienteId,msg,sizeof(int),0);

            if (byteslidos == -1)
            {
                printf("Falha ao executar recv()");
                exit(EXIT_FAILURE);
            }
            else if (byteslidos == sizeof(int))
            {
                first = *((int*) msg);
                printf("Servidor recebeu a seguinte msg do cliente: %d \n",first);

                cout<<first<<endl;

                if(first==0) toggle_all();
                if(first<0)
                {
                    switch(first)
                    {
                        case -1: gtrem1->setEnable( !gtrem1->getEnable() );
                        break;
                        case -2: gtrem2->setEnable( !gtrem2->getEnable() );
                        break;
                        case -3: gtrem3->setEnable( !gtrem3->getEnable() );
                        break;
                        case -4: gtrem4->setEnable( !gtrem4->getEnable() );
                        break;
                        case -5: gtrem5->setEnable( !gtrem5->getEnable() );
                        break;
                        case -6: gtrem6->setEnable( !gtrem6->getEnable() );
                        break;
                    }
                }
                if(first>0)
                {
                    if(!finitestate)
                    {
                        finitestate=first;
                    }
                    else
                    {
                        switch(finitestate)
                        {
                            case 1: gtrem1->setVelocidade(first);
                            break;
                            case 2: gtrem2->setVelocidade(first);
                            break;
                            case 3: gtrem3->setVelocidade(first);
                            break;
                            case 4: gtrem4->setVelocidade(first);
                            break;
                            case 5: gtrem5->setVelocidade(first);
                            break;
                            case 6: gtrem6->setVelocidade(first);
                            break;
                        }
                        finitestate=0;
                    }
                }
            }
        }

        close(conexaoClienteId);
    }
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Semaforo *sem[8];
    sem[0] = new Semaforo(1200, 1, 0);
    sem[1] = new Semaforo(1201, 1, 0);
    sem[2] = new Semaforo(1202, 1, 0);
    sem[3] = new Semaforo(1203, 1, 0);
    sem[4] = new Semaforo(1204, 1, 0);
    sem[5] = new Semaforo(1205, 1, 0);
    sem[6] = new Semaforo(1206, 1, 0);
    sem[7] = new Semaforo(1207, 1, 0);

    cout<<"Sobrevivi"<<endl;

    gtrem1 = trem1 = new Trem(1,130,320,sem);
    connect(trem1,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trem1->start();

    gtrem2 = trem2 = new Trem(2,400,320,sem);
    connect(trem2,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trem2->start();

    gtrem3 = trem3 = new Trem(3,170,160,sem);
    connect(trem3,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trem3->start();

    gtrem4 = trem4 = new Trem(4,560,20,sem);
    connect(trem4,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trem4->start();

    gtrem5 = trem5 = new Trem(5,310,20,sem);
    connect(trem5,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trem5->start();

    gtrem6 = trem6 = new Trem(6,120,20,sem);
    connect(trem6,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    trem6->start();

    std::thread *servidor_thread = new std::thread(servidor);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateInterface(int id, int x, int y)
{
    switch(id){
        case 1:
            ui->labelTrem01->setGeometry(x,y,20,20);
            break;
        case 2:
            ui->labelTrem02->setGeometry(x,y,20,20);
            break;
        case 3:
            ui->labelTrem03->setGeometry(x,y,20,20);
            break;
        case 4:
            ui->labelTrem04->setGeometry(x,y,20,20);
            break;
        case 5:
            ui->labelTrem05->setGeometry(x,y,20,20);
            break;
        case 6:
            ui->labelTrem06->setGeometry(x,y,20,20);
            break;
        default:
            break;
    }
}
