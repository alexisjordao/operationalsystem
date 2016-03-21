#include<unistd.h>
#include<cstdlib>
#include<sys/types.h>
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
#include<algorithm>

using namespace std;

vector<string> listFile(){
        vector<string> retorno;

        DIR *pDIR;
        struct dirent *entry;
        if( pDIR=opendir("./") ){
                while(entry = readdir(pDIR)){
                        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                        //cout << entry->d_name << "\n";
                        if(entry->d_name[0]=='f' &&  entry->d_name[1]=='i')
                            retorno.push_back(entry->d_name);
                }
                closedir(pDIR);
        }
        return retorno;
}
int main()
{
    ifstream myfile;
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
        cout << retorno;
        myfile.close();

        saidas.push_back(make_pair(timestamp,retorno));
    }

    sort(saidas.begin(), saidas.end());

    for(auto a : saidas)
    {
        cout << a.first << " " << a.second << endl;
    }


    return 0;
}