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

	char *instanceName = NULL;

	int numgeracoes;
	int numplasmideos1;
	int ini_ger_sett;
	float ppprob_vector;
	float ppprob_factor;
	int max_tabuu; 
	int tabutenuree;
	int max_vizinhoss;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-numgeracoes") == 0) {
            numgeracoes = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-numplasmideos1") == 0) {
            numplasmideos1 = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-ini_ger_sett") == 0) {
            ini_ger_sett = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-ppprob_vector") == 0) {
            ppprob_vector = atof(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-ppprob_factor") == 0) {
            ppprob_factor = atof(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-max_tabuu") == 0) {
            max_tabuu = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-tabutenuree") == 0) {
            tabutenuree = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-max_vizinhoss") == 0) {
            max_vizinhoss = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-instance") == 0 || strcmp(argv[i], "-i") == 0) {
            instanceName = argv[i+1];
            i++;
        }
    }

    int n; // esta leitura de "n" é somente para cumprir o formato da instância. Os valores de fato estao em param.h
	std::ifstream inn;
    inn.open(instanceName, std::ifstream::in);
	inn>>n; // quantidade de vertices
	//cout<<"n = "<<n<<endl;
	string cmd = "./trans_"+std::to_string(n)+" -i "+instanceName +" -numgeracoes "+std::to_string(numgeracoes)+" -numplasmideos1 "+std::to_string(numplasmideos1)+" -ini_ger_sett "+std::to_string(ini_ger_sett)+" -ppprob_vector "+std::to_string(ppprob_vector)+" -ppprob_factor "+std::to_string(ppprob_factor)+" -max_tabuu "+std::to_string(max_tabuu)+" -tabutenuree "+std::to_string(tabutenuree)+" -max_vizinhoss "+std::to_string(max_vizinhoss);
	//cout<<cmd<<endl;
	system(cmd.c_str());
    return 0;
    //g++ main.cpp -Wall -O3 -o main
}