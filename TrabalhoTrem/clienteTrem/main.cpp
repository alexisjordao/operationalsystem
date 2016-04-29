#include "../../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"
#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#define MAXMSG 1024
#define PORTNUM 4325

#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"

int readAnalog(int number){
   stringstream ss;
   ss << PATH_ADC << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

int readNumber(int number) {
    BlackLib::BlackGPIO   botao(BlackLib::GPIO_115,BlackLib::input, BlackLib::SecureMode);
    int valorbotao;
    valorbotao = botao.getNumericValue();
    cout << "Valor do botão é " << valorbotao << endl;
    return number;
}

const char mensagens[][50] = {"","Cliente ainda nao conectado ao servidor","Cliente conectado com sucesso","Cliente foi desconectado","Erro no connect","Erro ao enviar dados","Mensagem enviada com sucesso"};
int mensagem=0;


int state = 0;
int argumento;

// 0 menu inicial
// 1 escolha de trem

int escolha;
int opcao=1;

struct sockaddr_in endereco;
int socketId;

int conectado=0;


void opensocket()
{
     int valor = readAnalog(1);
     float *msg = new float((valor*1.8)/4096);

     int bytesenviados;

     memset(&endereco, 0, sizeof(endereco));
     endereco.sin_family = AF_INET;
     endereco.sin_port = htons(PORTNUM);
     endereco.sin_addr.s_addr = inet_addr("127.0.0.1");

     socketId = socket(AF_INET, SOCK_STREAM, 0);

     if (socketId == -1)
     {
         mensagem=4;
         return;
     }
}

void opcao1()
{
    int *msg=new int(0);
    int bytesenviados = send(socketId,(void*) msg,sizeof(int),0);

    if (bytesenviados == -1)
    {
        mensagem = 5;
        return;
    }
    mensagem = 6;
}

void opcao2(int trem)
{
     int *msg=new int(-trem);
     int bytesenviados = send(socketId,(void*) msg,sizeof(int),0);

     if (bytesenviados == -1)
     {
         mensagem = 5;
         return;
     }
     mensagem = 6;
}

void opcao3(int trem)
{
     int *msg=new int(trem);
     int bytesenviados = send(socketId,(void*) msg,sizeof(int),0);

     if (bytesenviados == -1)
     {
         mensagem = 5;
         return;
     }

     msg=new int(40);
     bytesenviados = send(socketId,(void*) msg,sizeof(int),0);

     if (bytesenviados == -1)
     {
          mensagem = 5;
          return;
     }

     mensagem = 6;
}

void opcao4()
{
    if(conectado)
    {
        close(socketId);
        mensagem=3;
        conectado=0;
    }
    else
    {
        opensocket();
        if ( connect (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
        {
            mensagem=4;
            return;
        }
        mensagem = 2;
        conectado = 1;
    }
}

void printMenu()
{
    system("clear");

    if(state==0)
    {
        if(opcao==1) cout<<" x "; else cout<<"   ";
        cout<<"Parar/resumir todos os trens"<<endl;
        if(opcao==2) cout<<" x "; else cout<<"   ";
        cout<<"Parar/resumir trem específico"<<endl;
        if(opcao==3) cout<<" x "; else cout<<"   ";
        cout<<"Alterar velocidade de trem"<<endl;
        if(opcao==4) cout<<" x "; else cout<<"   ";
        cout<<"Conectar/desconectar de servidor"<<endl;
    }
    else
    {
        if(opcao==1) cout<<" x "; else cout<<"   ";
        cout<<"Trem 1"<<endl;
        if(opcao==2) cout<<" x "; else cout<<"   ";
        cout<<"Trem 2"<<endl;
        if(opcao==3) cout<<" x "; else cout<<"   ";
        cout<<"Trem 3"<<endl;
        if(opcao==4) cout<<" x "; else cout<<"   ";
        cout<<"Trem 4"<<endl;
        if(opcao==5) cout<<" x "; else cout<<"   ";
        cout<<"Trem 5"<<endl;
        if(opcao==6) cout<<" x "; else cout<<"   ";
        cout<<"Trem 6"<<endl;
    }

    cout<<endl<<mensagens[mensagem];
    mensagem=0;
}

void down()
{
    opcao++;
    if(state == 0 && opcao == 5) opcao=1;
    if(state == 1 && opcao == 7) opcao=1;

    printMenu();
}

void up()
{
    opcao--;
    if(state == 0 && opcao == 0) opcao=4;
    if(state == 1 && opcao == 0) opcao=4;

    printMenu();
}

void play()
{
    if(state==0)
    {
        switch(opcao)
        {
            case 1:opcao1(); break;
            case 2:state=1;argumento=opcao;opcao=1; break;
            case 3:state=1;argumento=opcao;opcao=1; break;
            case 4:opcao4(); break;
        }
    }
    else
    {
        cout<<argumento<<"!"<<endl;
        if(argumento==2)
        {
            opcao2(opcao);
        }
        else if(argumento==3)
        {
            opcao3(opcao);
        }
        state=0;opcao=1;
    }
    printMenu();
}


int main(int argc, char *argv[])
{

    printMenu();
    while( escolha = getchar() - '0')
    {
        switch(escolha)
        {
            case 1: up(); break;
            case 2: down(); break;
            case 3: play(); break;
        }
    }

    return 0;
}
