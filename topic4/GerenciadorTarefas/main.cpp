#include "mainwindow.h"
#include<QApplication>
#include<stdio.h>
#include<string>
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
using namespace std;

ofstream json;



bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

vector<string> listFile(){
    vector<string> retorno;

    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir("/proc/") ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            //cout << entry->d_name << "\n";
            //if(entry->d_name[0]=='f' &&  entry->d_name[1]=='i' &&  entry->d_name[2]=='l' &&  entry->d_name[3]=='h')
            if ( is_number(entry->d_name))
            retorno.push_back(entry->d_name);
        }
        closedir(pDIR);
    }
    return retorno;
}

int cpu()
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

int memoria()
{
    ifstream myfile;
    myfile.open("/proc/statm");

    string teste;
    int mem;

    myfile >> mem;

    return mem;
}

vector<int> listChildren(int parent)
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

string getPName(int pid)
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



int memorybypid(int pid)
{
    stringstream ss;
    ss << "/proc/" << pid << "/statm";

    ifstream myfile;
    myfile.open(ss.str().c_str());

    int retorno;

    myfile >> retorno;

    return retorno;
}

void recursiveJSon(int processo,int nivel)
{
    vector<int> children = listChildren(processo);

    json<<string(nivel*4,' ')<<"{"<<endl;
    json<<string(nivel*4,' ')<<"\"name\" : \""<< getPName(processo) <<"\","<<endl;
    json<<string(nivel*4,' ')<<"\"children\":"<<endl;
    json<<string(nivel*4,' ')<<"["<<endl;
    json<<string(nivel*4,' ')<<"{\"name\": \""<<getPName(processo)<<"\", \"size\": "<<memorybypid(processo)/1000<<", \"pid\": "<<processo<<"}"<<endl;

    for(auto x:children)
    {
        json<<","<<endl;
        recursiveJSon(x,nivel+1);
    }

    json<<string(nivel*4,' ')<<"]"<<endl;


    json<<string(nivel*4,' ')<<"}";

}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //for(auto x : listFile())
    //    cout << x << endl;

    json.open ("/home/jordao/operationalsystems/teste/processos.json");
    recursiveJSon(1,0);
    json.close();



    //for(auto x : listChildren("2"))
    //    cout << x << endl;

    //cout<<getPName(2)<<endl;

    return a.exec();
    return 0;
}
