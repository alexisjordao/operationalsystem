#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>

// Identificador do processo
pid_t idProcesso;

int state;
pid_t child;

void detectarAlarme (int sig){
  if(state==0)
  {
     kill(idProcesso,SIGWINCH);
     alarm(3);
  }
  else if(state==1)
  {
     kill(idProcesso,SIGUSR1);
     alarm(2);
  }
  else if(state==2)
  {
     kill(idProcesso,SIGUSR2);
     alarm(4);
  }

  state=++state%3;
} 

void funcaoSignalHandler (int sig)          //jordao vc precisa mexer aqui
{
  switch(sig)
  {
    case SIGWINCH: printf("Vermelha\n"); break;
    case SIGUSR1: printf("Amarela\n"); break;
    case SIGUSR2: printf("Verde\n"); break;
  }
}

void interruption (int sig)
{
   state=0;
   detectarAlarme(SIGALRM);
}

void ignore(int sig){}

void ctrlz(int sig)
{
  exit(0);
}
 
int main(int argc, char *argv[])
{
    state=0;
    printf("Iniciando o programa\n");
 
    signal(SIGWINCH, funcaoSignalHandler);
    signal(SIGUSR1, funcaoSignalHandler);
    signal(SIGUSR2, funcaoSignalHandler);
    signal(SIGTSTP, ctrlz);
 
    idProcesso = fork();
    switch(idProcesso){
      case -1:       
        exit(1);
      case 0:   
        signal(SIGINT, ignore);
        while(1){
          sleep(1);
        }
        break;
      default:  
        signal(SIGINT, interruption);
        signal(SIGALRM,detectarAlarme);
        detectarAlarme(SIGALRM);
        while(1){
          sleep(1);
        }
        break;
    }
 
    return 0;
}
