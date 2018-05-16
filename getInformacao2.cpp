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
	 char  buffer[1026];
	 if (entrada==NULL ) return 1; //|| saida==NULL)
	//float valor;
	char* padrao1 = "Quantidade total de invocacao da BT = ";
	char* padrao2 = "Quantidade total de sucesso da BT = ";
	float quantidadeDeAtaques, quantidadeDeSucesso;
	  while( fgets(buffer,1025,entrada) != NULL ){
        if( strstr(buffer,padrao1) != NULL ){
             //printf("%s : ",file);
        	// cout<<buffer<<endl;
			 char *  rrr = strstr(buffer, "=");
			 char *  rrr2 = strstr(rrr, " ");
			 // cout<<rrr2<<endl;
        	quantidadeDeAtaques = atof(rrr2);
                    
    	}

    	 if( strstr(buffer,padrao2) != NULL ){
    	 	char *  rrr = strstr(buffer, "=");
			 char *  rrr2 = strstr(rrr, " ");
			 // cout<<rrr2<<endl;
        	quantidadeDeSucesso = atof(rrr2);
        	// cout<<"quantidadeDeSucesso = "<<quantidadeDeSucesso<<endl;
    	 }
    }
    //quantidadeDeSucesso/quantidadeDeAtaques
    if (quantidadeDeAtaques==0) cout<<"0"<<endl;
    cout<<(float)quantidadeDeSucesso/quantidadeDeAtaques<<endl;
	return 0;

}