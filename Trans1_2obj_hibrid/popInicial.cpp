#ifndef POP_INICIAL
#define POP_INICIAL

#include "param.h"
#include "SolucaoEdgeSet.cpp"

#include <iostream>
using namespace std;


extern BoundedParetoSet arc_global;

// aloca memoria para os individuos da populaçao
void alocaPopulacao(SolucaoEdgeSet *populacao[TAMANHOPOPULACAO]){
	for (int i=0; i<TAMANHOPOPULACAO; i++){
		populacao[i] = new SolucaoEdgeSet(NUMEROVERTICES-1);
	}
}

/*
	População inicial é gerada conforme sugerido por Monteiro (2010)
	cria no maximo 90% da populaçao com o rmcprim (NAO CONSIDERA nem individuos repetidos nem dominados)
	cria no minimo 10% da populaçao com o randomWolk (CONSIDERA individuos repetidos e dominados)
*/
void gerarPopulacao1(SolucaoEdgeSet *populacao[TAMANHOPOPULACAO]){
	double lambda[NUMOBJETIVOS] = {0.0, 1.0};
	int quant_prim = (int)(TAMANHOPOPULACAO*0.9); // 90% com rmc_prim
	double factor = (quant_prim==0 ? 0 : 1.0/quant_prim); // fator de incremetar o lambda
	int contPop = 0; // contador para indexar o vetor populacao
	while (lambda[0]<1.0){
		rmcPrim( *populacao[contPop], lambda);
		lambda[0]+=factor;
		lambda[1]-=factor;

		bool ha = false;
		for (int j=0;j<contPop && ha==false;j++) {
			if (*populacao[contPop] == *populacao[j]) { // note que aqui vericamos o conteudo, e nao o ponteiro
				ha = true;
			}
		}

		if (ha==false){ 
			bool resp = arc_global.adicionarSol(populacao[contPop]);
			if (resp==true){ // aceita se pop[contPop] nao for dominada por ninguém no arquivo
				contPop++;
			} else if (genrand64_real3() < 0.6) { // aceita com 60% de chance
				contPop++;
			} 
		}
	}
	
	while (contPop<TAMANHOPOPULACAO){
		populacao[contPop]->doRandomWalk();
		arc_global.adicionarSol(populacao[contPop]);
		contPop++;
	}
}

#endif