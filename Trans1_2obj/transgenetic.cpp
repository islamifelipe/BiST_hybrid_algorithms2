/* 
=====================================================================================

Copyright Islame Felipe da COSTA FERNANDES (2018)
	
This code implements a new transgenetic algorithmic for Bi-objective spanning tree 
The data structure and some functions were kindly provided by Monteiro (2010)


=====================================================================================
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

#include "BoundedParetoSet.cpp"
#include "SolucaoEdgeSet.cpp"
#include "rmcPrim.cpp"
#include "popInicial.cpp"
// #include "vetoresDirecao.cpp"
// #include "simulatedannealing.cpp"
// #include "Plasmideo.cpp"
// #include "primTransposon.cpp"
// #include "kruskalTransposon.cpp"

using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
SolucaoEdgeSet *populacao[TAMANHOPOPULACAO];
BoundedParetoSet arc_global;

struct tms tempoAntes, tempoDepois;


void input(){
	int n; // esta leitura de "n" é somente para cumprir o formato da instância. Os valores de fato estao em param.h
	cin>>n; // quantidade de vertices

	int org, dest;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			cin>>org;
			cin>>dest;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				cin>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j]; // a parte superior da matriz é refletida na inferior
			}
		}
	}
}

int main(int argc, char *argv[]){
	int seemente = std::atoi(argv[1]);
	init_genrand64(seemente);
	cout<<"========= Estatisticas ========= "<<endl;
	cout<<"Semente utilizada : "<<seemente<<endl;
	FILE *samplefile = fopen(argv[2],"a");
	FILE *tempofile = fopen(argv[3],"a");
	input(); // ler instância
	cout<<"Instância lida..."<<endl;

	times(&tempoAntes);
	
	alocaPopulacao(populacao);
	gerarPopulacao1(populacao);

	cout<<"Popualaçao: "<<endl;
	for (int i=0; i<TAMANHOPOPULACAO; i++){
		cout<<populacao[i]->getObj(0)<<" "<<populacao[i]->getObj(1)<<endl;
	}

	cout<<"\nArquivo: "<<endl;
	for (int i=0; i<arc_global.getSize(); i++){
		SolucaoEdgeSet *sol = arc_global.getSolucao(i);
		cout<<sol->getObj(0)<<" "<<sol->getObj(1)<<endl;
	}

	cout<<"\nLixeira: "<<endl;
	list<SolucaoEdgeSet *> lixeira = arc_global.lixeira;
	list<SolucaoEdgeSet *>::iterator it = lixeira.begin();
	while (it!=lixeira.end()){
		cout<<(*it)->getObj(0)<<" "<<(*it)->getObj(1)<<endl;
		it++;
	}

	times(&tempoDepois);

	fprintf(stdout,"Tempo(s) Final = %.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
	fprintf(tempofile,"%.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );


	fclose(samplefile);
	fclose(tempofile);

	// cout<<NUMEROVERTICES<<endl;
	// for (int i=0; i<NUMEROVERTICES-1; i++){
	// 	for (int j=i+1; j<NUMEROVERTICES; j++){
	// 		cout<<i<<" "<<j<<" "<<custos[0][j][i]<<" "<<custos[1][j][i]<<endl;
	// 	}	
	// }

}