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
	Este programa recebe um arquivo contendo o resultado de um teste estatistico
	extrai o p-valor e coloca no arquivo de saida . Deve ser usado em conjunto com algum script shell
*/


int main (int argc, const char * argv[]) { 

	 FILE * entrada = fopen (argv[1], "r");
	  // FILE * saida = fopen (argv[2], "a");
	 char  buffer[1026];
	 if (entrada==NULL ) return 1; //|| saida==NULL)
	//float valor;
	char* padrao = "1 better than 2 with a p-value of ";
	  while( fgets(buffer,1025,entrada) != NULL ){
        if( strstr(buffer,padrao) != NULL ){
             //printf("%s : ",file);
			 char *  rrr = strstr(buffer, "  ");
        	float rfgt = atof(rrr);
        	// fprintf(saida,"%f", rfgt);;
        	cout<<rfgt<<" ";
        	//fscanf(entrada, "%f", &valor);
        	break;
                    
    	}
    }
	
	return 0;

}