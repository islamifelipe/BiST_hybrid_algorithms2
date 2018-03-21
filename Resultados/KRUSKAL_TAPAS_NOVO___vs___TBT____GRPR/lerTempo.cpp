#include <iostream>
#include <cstring>
#include <math.h>
#include <list>
#include <vector>
#include <stdio.h>
#include <utility>      // std::pair
#include <string.h>
using namespace std;


int main (int argc, const char * argv[]) { 

	 FILE * entrada = fopen (argv[1], "r");
	 // FILE * saida = fopen (argv[2], "a");
	 if (entrada==NULL) return 1;
	//float valor;
	 
	 float tempoIN, tempo=0;
	 do{
		if (feof(entrada)==false){
		 fscanf(entrada, "%f", &tempoIN);
		 tempo+=tempoIN;
		 //cout<<tempoIN<<endl;
		}
	}while (feof(entrada)==false);

	 tempo = (tempo-tempoIN)/30.0;
	 //cout<<tempo<<endl;
	 cout<<tempo<<" ";;
	
	return 0;

}