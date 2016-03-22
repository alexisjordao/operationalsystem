//#include "../../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"
#include<unistd.h>
#include<cstdlib>
#include<sys/types.h>
#include <sys/wait.h>
#include<unistd.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<ctime>
#include<utility>
#include<vector>
#include<sstream>
#include<dirent.h>
#include<fstream>
#include<cstring>
#include<algorithm>

//#include<>
using namespace std;

vector<string> listFile(){
    vector<string> retorno;
    
    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir("./") ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            //cout << entry->d_name << "n";
            if(entry->d_name[0]=='f' &&  entry->d_name[1]=='i' &&  entry->d_name[2]=='l' &&  entry->d_name[3]=='h')
            retorno.push_back(entry->d_name);
        }
        closedir(pDIR);
    }
    return retorno;
}

//#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"
/*
int readAnalog(int number){
    stringstream ss;
    ss << PATH_ADC << number << "_raw";
    fstream fs;
    fs.open(ss.str().c_str(), fstream::in);
    fs >> number;
    fs.close();
    return number;
}*/

int main(int argc, char *argv[])
{
    ofstream myfile;
    
    // Identificador do processo
    pid_t idProcesso, idProcesso_2, idProcesso_3, idProcesso_4;
    
    //BlackLib::BlackGPIO   ledR(BlackLib::GPIO_60,BlackLib::output, BlackLib::SecureMode);
    //BlackLib::BlackGPIO   ledG(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
    //BlackLib::BlackGPIO   ledB(BlackLib::GPIO_51,BlackLib::output, BlackLib::SecureMode);
    
    cout<<"Iniciando o processo."<<endl;
    
    
    
    // Criando o processo
    idProcesso = fork();
    
    
    switch(idProcesso){
        case -1:       // erro na geração do processo filho
        exit(1);
        case 0:        // retorno de fork para processo filho 1
        for (int i=0;i<10;i++){
            //printf("Sou o processo filho ID: %dn",getpid());
            //ledR.setValue(BlackLib::high);
            //ledG.setValue(BlackLib::low);
            //ledB.setValue(BlackLib::low);
            sleep(2);
            //ledR.setValue(BlackLib::low);
            //ledG.setValue(BlackLib::low);
            //ledB.setValue(BlackLib::low);
            
            string fs;
            stringstream ss;
            ss<<"filho_1_";
            ss<<i;
            ss<<".txt";
            ss>>fs;
            
            myfile.open (fs.c_str());
            myfile << time(NULL) << endl << "Vou ligar o LED vermelho.n";
            myfile.close();
        }
        
        return 0;
        break;
        default:
        idProcesso_2 = fork();
        switch(idProcesso_2){
            case -1:       // erro na geração do processo filho
            exit(1);
            case 0:        // retorno de fork para processo filho 2
            for (int i=0;i<10;i++){
                //printf("Sou o processo filho ID: %dn",getpid());
                //ledR.setValue(BlackLib::low);
                //ledG.setValue(BlackLib::high);
                //ledB.setValue(BlackLib::low);
                sleep(3);
                //ledR.setValue(BlackLib::low);
                //ledG.setValue(BlackLib::low);
                //ledB.setValue(BlackLib::low);
                
                string fs;
                stringstream ss;
                ss<<"filho_2_";
                ss<<i;
                ss<<".txt";
                ss>>fs;
                
                myfile.open (fs.c_str());
                myfile << time(NULL) << endl << "Vou ligar o LED verde.n";
                myfile.close();
            }
            return 0;
            break;
            default:
            idProcesso_3 = fork();
            
            switch(idProcesso_3){
                case -1:       // erro na geração do processo filho
                exit(1);
                case 0:        // retorno de fork para processo filho 3
                for (int i=0;i<10;i++){
                    //printf("Sou o processo filho ID: %dn",getpid());
                    //ledR.setValue(BlackLib::low);
                    //ledG.setValue(BlackLib::low);
                    //ledB.setValue(BlackLib::high);
                    sleep(4);
                    //ledR.setValue(BlackLib::low);
                    //ledG.setValue(BlackLib::low);
                    //ledB.setValue(BlackLib::low);
                    
                    string fs;
                    stringstream ss;
                    ss<<"filho_3_";
                    ss<<i;
                    ss<<".txt";
                    ss>>fs;
                    
                    myfile.open (fs.c_str());
                    myfile << time(NULL) << endl << "Vou ligar o LED azul.n";
                    myfile.close();
                }
                return 0;
                break;
                default:
                idProcesso_4 = fork();
                switch(idProcesso_4){
                    case -1:       // erro na geração do processo filho
                    exit(1);
                    case 0:        // retorno de fork para processo filho 4
                    for (int i=0;i<10;i++){
                        //printf("Sou o processo filho ID: %dn",getpid());
                        // int valor = readAnalog(1);
                        //cout << "O valor lido foi " <<  " [0 a 4095]." << endl;
                        //cout << "O valor corresponde a V" << endl;
                        
                        sleep(1);
                        
                        string fs;
                        stringstream ss;
                        ss<<"filho_4_";
                        ss<<i;
                        ss<<".txt";
                        ss>>fs;
                        
                        myfile.open (fs.c_str());
                        myfile << time(NULL) << endl << "O valor do potenciometro é: .n";
                        myfile.close();
                    }
                    return 0;
                }
            }
            
            cout<<"Processos criados: "<<idProcesso<<" "<<idProcesso_2<<" "<<idProcesso_3<<" "<<idProcesso_4<<endl;
            
            int status;
            
            cout<<"Esperando morte dos descendentes..."<<endl;
            while(wait(&status) > 0) { /* no-op */ ; }
            
            vector<pair<int,string> > outputs;
            
            
            ifstream myfile;
            ofstream resultado;
            vector<string> filhos = listFile();
            vector<pair<int,string> > saidas;
            for(string a : filhos)
            {
                string retorno;
                int timestamp;
                
                myfile.open (a);
                myfile >> timestamp;
                getline(myfile, retorno); //excluding carriage return!!! sorry
                getline(myfile, retorno);
                myfile.close();
                
                saidas.push_back(make_pair(timestamp,retorno));
            }
            
            sort(saidas.begin(),saidas.end());
            
            resultado.open ("saida.txt");
            
            for(auto a : saidas)
            {
                resultado << "Timestamp: " <<a.first << " | Mensagem: " << a.second << endl;
            }
            
            resultado.close();
            
            
            cout<<"Cheque o resultado em saida.txt"<<endl;
            return 0;
        }
    }
}
