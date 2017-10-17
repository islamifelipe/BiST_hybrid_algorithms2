/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017.2
#-----------------------------------------------------------------------
# This code implements a hybrid GRASP algorithm with Path Relinking
#=======================================================================


	calcula os vetores de escalarizacao
	Grasp{ (Baseado em parte no trabalho de Arroyo e Vianna)
		Constroi solucao 
		Busca local (Vizinhança: remove duas arestas e religa como no 2-OPT. Baseado em Rocha, Goldbarg e Goldbarg (2006))
			==> A parte grasp é baseada no trabalho de Goldbarg, Goldbarg e Farias (2007) (aplicou ao caixeiro)
		Após INTERVALO_PR iteraçoes do grasp, monta-se um pool com INTERVALO_PR soluçoes
		Em sequigda, aplica-se o PR truncado (Resende e Ribeiro (2005) e Laguna and Marti (1999)) no pool
		Toma-se pares de soluçoes do pool que estao mais proximas
		métrica para dizer se duas soluçoes do pool sao proximas: 
	}

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

 
// #include "rmcPrim.cpp"
#include "BoundedParetoSet.cpp"
#include "SolucaoEdgeSet.cpp"

using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
BoundedParetoSet *arquivoLimitadoGlobal;

/*ARQUIVO GLOBAL LIMITADO A MAXARCSIZE SOLUCOES*/

void input(){
	int n; // esta leitura de n é somente para cumprir o formato da instância. O valore de fato está em param.h
	cin>>n;
	int org, dest;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			cin>>org;
			cin>>dest;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				cin>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j];
			}
		}
	}
}





/*semente, arquivo pareto, tempo, lixeira*/

 int main(int argc, char *argv[]){
	int seemente = std::atoi(argv[1]);
	TRandomMersenne rg(seemente);
	cout<<"========= Estatisticas ========= "<<endl;
	cout<<"Semente utilizada : "<<seemente<<endl;
	FILE *samplefile = fopen(argv[2],"a");
	FILE *tempofile = fopen(argv[3],"a");
	char *lixeira = argv[5];
	input(); // ler instância

	arquivoLimitadoGlobal = new BoundedParetoSet();
	arquivoLimitadoGlobal->setNomeGlobalf(lixeira);

	

	arquivoLimitadoGlobal->printSetPoints(stdout);



	fclose(samplefile);
	fclose(tempofile);
}