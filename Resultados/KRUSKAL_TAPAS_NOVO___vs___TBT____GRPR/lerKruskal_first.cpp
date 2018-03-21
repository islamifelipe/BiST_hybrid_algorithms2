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
	
	ESTE PROGRAMA LER OS P_VALORES DO KRUSKAL_WALLIS REFERENTES A PRIMEIRA FASE 
	SE MENOR QUE 0.05, SIGNIFICA QUE A HIPOTESE NULA PODE SER REJEITADA E QUE
	HA DIFERENCA SIGNIFICATIVA ENTRE OS TRES ALGORITMOS

*/


int main (int argc, const char * argv[]) { 

	 FILE * entrada = fopen (argv[1], "r");
	  // FILE * saida = fopen (argv[2], "a");
	 char  buffer[1026];
	 if (entrada==NULL ) return 1; //|| saida==NULL)
	//float valor;
	char* padrao = "p-value to accept the null hypothesis that all distribution functions are identical =";
	  while( fgets(buffer,1025,entrada) != NULL ){
	  	 // cout<<buffer<<endl;
        if( strstr(buffer,padrao) != NULL ){
             //printf("%s : ",file);
			 char *  rrr = strstr(buffer, "=");
			 char *  rrr2 = strstr(rrr, " ");
			 if (strstr(rrr2,"nan")!=NULL) cout<<"1"<<" ";
        	 else { 
        	 		float rfgt = atof(rrr2);
        			cout<<rfgt<<" ";
        	}
        	//fscanf(entrada, "%f", &valor);
        	break;
                    
    	}
    }
	
	return 0;

}