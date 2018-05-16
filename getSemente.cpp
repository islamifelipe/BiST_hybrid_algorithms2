#include <iostream>
#include <cstring>
#include <math.h>
#include <list>
#include <vector>
#include <stdio.h>
#include <utility>      // std::pair
#include <string.h>
using namespace std;

/*
	ESTE PROGRAMA LER UM ARQUIVO DO TAPAS OU DO TBTv1 E RETORNA A SEMENTE
*/


int main (int argc, const char * argv[]) { 

	 int kkk = atoi(argv[1]);; //de 50 a 1000
	 int iii = atoi(argv[2]);; //1 2 3
	 int ccc = atoi(argv[3]);; //de 1 a 30
	 FILE * entrada = fopen (argv[4], "r");
	 FILE * saida = fopen (argv[5], "a");
	 int semente;
	  // FILE * saida = fopen (argv[2], "a");
	 char  buffer[1026];
	 if (entrada==NULL ) return 1; //|| saida==NULL)
	//float valor;
	char* padrao = "Semente utilizada : ";
	  while( fgets(buffer,1025,entrada) != NULL ){
        if( strstr(buffer,padrao) != NULL ){
             //printf("%s : ",file);
        	// cout<<buffer<<endl;
			 char *  rrr = strstr(buffer, ":");
			 char *  rrr2 = strstr(rrr, " ");
			 // cout<<rrr2<<endl;
        	int rfgt = atoi(rrr2);
        	semente = rfgt;

        	break;
                    
    	}
    }

    // cout<<semente<<endl;
   fprintf(saida,"../../memetic %d concave/%d.conc%d/pareto1_%d.conc%d.out concave/%d.conc%d/pareto2_%d.conc%d.out concave/%d.conc%d/tempo_%d.conc%d.out < ../../../InstanciasMestrado/INST1/concave/%d.conc%d.in > concave/%d.conc%d/%d.conc%d_%d.out\n", semente, kkk, iii,kkk, iii,kkk, iii,kkk, iii,kkk, iii,kkk, iii,kkk, iii,kkk, iii,kkk, iii,  ccc);
		
		// ../../memetic 8062684 pareto1_50.conc1.out tempo_50.conc1.out < ../../../InstanciasMestrado/INST1/concave/50.conc1.in > concave/50.conc1/50.conc1_1.out

	return 0;

}