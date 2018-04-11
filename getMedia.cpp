
/*Este programa lê o aquivo com N valores  (por exemmplo, 30 execuçoes)
e retorna:

media 		mediana		minima*/


#include <iostream>
#include <vector> 
#include <string>
#include <climits>
#include <stack>   
#include <utility>      // std::pair
#include <cmath>
#include <list>
#include <sys/times.h>
#include <unistd.h>
#include <stdlib.h>     /* srand, rand */
#include <algorithm>    // std::sort
#include <vector>       // std::vector
using namespace std;

#define N 30

bool myfunction (double i,double j) { return (i<j); }

int main(int argc, const char * argv[]){
	
	double val;
	std::vector<double> valores;
	double sum = 0.0;
	double min = 394895845958.0;
	for (int i=0; i<N; i++) {
		cin>>val;
		valores.push_back(val);
		sum+=val;
		if (val<min){
			min = val;
		}
	}
	sort(valores.begin(), valores.end(), myfunction);
	double mediana = (valores[N/2-1] + valores[N/2])/2.0; // -1 porque o index começa do zero. Se fosse do 1, seria [N/2] + [N/2-1]
	double media = sum/N;
	//cout<<media<<endl;//", ,"<<mediana<<", ,"<<min<<endl;
	cout<<media<<",";
	//cout<<mediana<<endl;
}
