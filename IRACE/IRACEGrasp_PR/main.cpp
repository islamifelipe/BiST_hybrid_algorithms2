/*
chama o transgenético (IRACE)
*/

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sys/times.h>
#include <sys/types.h>
#include <iostream>
#include <math.h>       /* acos, cos */
#include <climits>
#include <fstream>      // std::ifstream 

using namespace std;

int main(int argc, char *argv[]){

	int numvetores_graaspp; //NUMVETORES_GRASP
    int intervalo_prrr; //INTERVALO_PR
    int maax_lss; //MAX_LS
    int maxx_viz; //MAX_VIZ
    int maxprr; //maxPR

    char *instanceName = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-NUMVETORES_GRASP") == 0) {
                numvetores_graaspp = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-INTERVALO_PR") == 0) {
                intervalo_prrr = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-MAX_LS") == 0) {
                maax_lss = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-MAX_VIZ") == 0) {
                maxx_viz = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-maxPR") == 0) {
                maxprr = atoi(argv[i+1]);
                i++;
        }
        
        if (strcmp(argv[i], "-instance") == 0 || strcmp(argv[i], "-i") == 0) {
                instanceName = argv[i+1];
                i++;
        }
    }

    int n; 
    std::ifstream inn;
    inn.open(instanceName, std::ifstream::in);
	inn>>n; // quantidade de vertices
	string cmd = "./grapspr_"+std::to_string(n)+" -i "+instanceName +" -NUMVETORES_GRASP "+std::to_string(numvetores_graaspp)+" -INTERVALO_PR " +std::to_string(intervalo_prrr) + " -MAX_LS "+ std::to_string(maax_lss) +" -MAX_VIZ "+ std::to_string(maxx_viz) +" -maxPR "+std::to_string(maxprr);
	//cout<<cmd<<endl;
	system(cmd.c_str());
    return 0;
    //g++ main.cpp -Wall -O3 -o main
}