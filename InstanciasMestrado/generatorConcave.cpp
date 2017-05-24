/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Knowles's (2002) graph generator, 
# to generate concave 2-objetives complete graphs  
#=======================================================================
*/

#include <iostream>
#include <stdlib.h>     /* atoi */
#include <time.h>       /* time */

using namespace std;

#define multiplicador 100000

int main (int argc, char *argv[]){
	srand (time(NULL));
	// double eta= 0.2, zeta = 0.05;
	// int divisor= multiplicador - (int)((1.0 - zeta)*multiplicador) + 1;
	// int ra =((rand()%(divisor)) + (int)((1.0 - zeta)*multiplicador)); // entre 1-zeta e 1
	// cout<<(double)ra/multiplicador<<endl;
	// cout<<(double)ra<<endl;

	int n=0; // quantidade de vertices
	double eta, zeta, obj1, obj2;
	n =  atoi(argv[1]); 
	eta = atof(argv[2]); 
	zeta =  atof(argv[3]);
	int a_eta = eta*multiplicador, a_zeta = zeta*multiplicador;
	cout<<n<<endl; // quantidade de vertices
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			cout<<i<<" "<<j<<" ";

			if (i==0 && j == 1){
				obj1 = zeta;
				obj2 = zeta;
			} else if (i==0 && j == 2 ){
				obj1 = 0;
				obj2 = 1-zeta;
			} else if (i==1 && j==2){
				obj1 = 1-zeta;
				obj2 = 0;
			} else if (i>2 && j>2){
				int a_obj1 = ((rand()%(a_eta - a_zeta + 1)) + a_zeta); // entre zeta e eta
				int a_obj2 = ((rand()%(a_eta - a_zeta + 1)) + a_zeta);
				obj1 = (double) a_obj1/multiplicador;
				obj2 = (double)a_obj2/multiplicador;
			} else if ((i<=2 && j>2) || (i>2 && j<=2)){
				int divisor= multiplicador - (int)((1.0 - zeta)*multiplicador) + 1;
				
				int a_obj1 = ((rand()%(divisor) + (int)((1.0 - zeta)*multiplicador))); // entre 1-zeta e 1
				int a_obj2 = ((rand()%(divisor) + (int)((1.0 - zeta)*multiplicador))); // entre 1-zeta e 1
				obj1 = (double)a_obj1/multiplicador;
				obj2 = (double)a_obj2/multiplicador;
			} else {
				cout<<"opsss"<<endl;
			}
			cout<<(int)(obj1*1000)<<" "<<(int)(obj2*1000)<<endl;
		}
	}
}
