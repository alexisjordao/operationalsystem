/*
 * Universidade Federal do Rio Grande do Norte
 * Instituto Metropole Digital
 *
 * copyright: Ivanovitch Silva
 * e-mail: ivan@imd.ufrn.br
 *
 * Topic: handling time in c++
 * Program: measure execution time of sorting algorithms
 */

#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;


//Vectors used by sorting methods
int *vectorQuickSort;
int *vectorBubbleSort;
int *vectorShellSort;
int size;

//Function used by qsort to compare two numbers
int compare_ints( const void* a, const void* b ) {
     int* arg1 = (int*) a;
     int* arg2 = (int*) b;
     if( *arg1 < *arg2 ) return -1;
     else if( *arg1 == *arg2 ) return 0;
     else return 1;
}

//Sorting algorithm bubble sort
void bubbleSort(int *vector, int size) {
  int aux;
  for (int i = 0; i < size-1; i++) {
     for (int j = 0; j < size-1; j++) {
        if (vector[j] > vector[j+1]) {
          aux = vector[j];
      vector[j] = vector[j+1];
      vector[j+1] = aux;
        }
     }
  }
}

//Sorting algorithm shell sort
void shellSort(int *num, int size) {
     int i, temp, flag = 1, numLength = size;
     int d = numLength;
     while (flag || (d > 1)) {                    // boolean flag (true when not equal to 0)
          flag = 0;                               // reset flag to 0 to check for future swaps
          d = (d+1) / 2;
          for (i = 0; i < (numLength - d); i++) {
               if (num[i + d] > num[i]) {
                      temp = num[i + d];         // swap positions i+d and i
                      num[i + d] = num[i];
                      num[i] = temp;
                      flag = 1;                  // tells swap has occurred
               }
          }
     }
     return;
}

//Note that the numbers are generated ramdomically based on a seed. If the same seed is given twice, the generated numbers will also be equal 
void generateVector(int sizeVector, int seed){
  srand (seed);
  vectorQuickSort = new int[sizeVector];
  vectorBubbleSort = new int[sizeVector];
  vectorShellSort = new int[sizeVector];	
  for (int i=0;i<sizeVector;i++){
    vectorQuickSort[i] =  rand()%100000;
    vectorBubbleSort[i] = rand()%100000;
    vectorShellSort[i] = rand()%100000;
  }
}

//Deallocate the vectors
void deallocateVector(){
	delete [] vectorQuickSort;
	delete [] vectorBubbleSort;
	delete [] vectorShellSort;
}

int main(int argc, char *argv[])
{
  int values[] = {1000, 10000, 25000, 50000, 100000, 200000, 300000, 400000, 500000};
  clock_t clock_1, clock_2;
  time_t time_1, time_2;	

for(int n = 0; n < 9; n++) {
	printf ("\n\nTotal of %i values\n",values[n]);
	for (int i = 1; i < 11; i++) {
		generateVector(values[n],i);
		//--------------------------------------------------------------------
		clock_1 = clock();
		time_1 = time(NULL);
		qsort (vectorQuickSort, values[n], sizeof(int), compare_ints);
		clock_2 = clock();
		time_2 = time(NULL);
		cout << "Quicksort time: " << time_2-time_1 << " clock: " << clock_2-clock_1 << endl;
		//--------------------------------------------------------------------
		clock_1 = clock();
		time_1 = time(NULL);
		bubbleSort(vectorBubbleSort,values[n]);
		clock_2 = clock();
		time_2 = time(NULL);
		cout << "Bubblesort time: " << time_2-time_1 << " clock: " << clock_2-clock_1 << endl;
		//--------------------------------------------------------------------
		clock_1 = clock();
		time_1 = time(NULL);
		shellSort(vectorShellSort,values[n]);
		clock_2 = clock();
		time_2 = time(NULL);
		cout << "Shellsort time: " << time_2-time_1 << " clock: " << clock_2-clock_1 << endl;
		deallocateVector();
	}
} 

  return 0;
}
