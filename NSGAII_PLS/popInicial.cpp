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
	ATENÇAO: os autores nao deixam claro como foi criada a populaçao inicial
	Assim, a populaçao foi criada completamente aleatoria  
*/
void gerarPopulacao1(SolucaoEdgeSet *populacao[TAMANHOPOPULACAO]){
	for (int p = 0; p < TAMANHOPOPULACAO; p++){
		for (int i=0; i<NUMEROVERTICES-2; i++){
			populacao[p]->pruffer[i] = IRandom(0, NUMEROVERTICES-1);
		}
		populacao[p]->convertToTree();
		// populacao[p]->isTree();
		// cout<<populacao[p]->getObj(0)<<" "<<populacao[p]->getObj(1)<<endl;
	}
}

#endif