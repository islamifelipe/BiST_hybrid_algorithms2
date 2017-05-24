/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Knowles's (2002) graph generator, 
# to generate the correlated and anticorrelated 3-objetives complete graphs  
#=======================================================================
*/

#include <iostream>
#include <stdlib.h>     /* atoi */
#include <time.h>       /* time */

using namespace std;

int main (int argc, char *argv[]){

	int n=0; // quantidade de vertices
	double alpha;
	n = atoi(argv[1]); // quantidade de vertices
	alpha =  atof(argv[2]);
	cout<<n<<endl; // quantidade de vertices
	srandom (time(NULL));
	//float teste = ((float)(random())/(float)(RAND_MAX) ) ;

	//int teste2 = ((random()%2) + 1) ;
	
	double betha, gama;
	betha = 0.5*((double)(1.0-alpha)); // Correçao do Chen et al (2007)
	if (alpha>=0){
		gama = betha;
	} else{
		gama = 1 - betha;
	}
	cout<<"betha = "<<betha<<" gama = "<<gama<<endl;
	cout<<"alpha = "<<alpha<<endl;
	//int cont = 0;
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			cout<<i<<" "<<j<<" ";
			float obj1 = ((float)(random())/(float)(RAND_MAX));

			int sinal = 1;
			if (((random()%2) + 1) == 2) sinal = -1;
			float ra = ((float)(random())/(float)(RAND_MAX));
			float obj2 = alpha*obj1 + betha + gama*ra*sinal;

			// sinal = 1;
			// if (((random()%2) + 1) == 2) sinal = -1;
			// ra = ((float)(random())/(float)(RAND_MAX));
			// float obj3 = alpha*obj1 + betha + gama*ra*sinal;

			cout<<(int)(obj1*100)<<" "<<(int) (obj2*100)<<endl;
			
		}
	}
	return 0;
}