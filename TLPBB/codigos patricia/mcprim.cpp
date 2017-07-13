#include "param.h"

#define INF 1e9
#define for(i,n) for (int i=0;i<n;i++)

void mcprim(SolucaoEdgeSet &s, double lambda[NUMOBJETIVOS], TRandomMersenne &rg) {
	bool arvore[NUMEROVERTICES]; // elementos ja colocados na arvore
	double mindist[NUMEROVERTICES]; // menor distancia ate cada no
	int cont = 0; // numero de elementos ja colocados
	int viz, ant[NUMEROVERTICES];
	double maisprox;
	
	int inicio = rg.IRandom(0,NUMEROVERTICES-1);
	
	// inicialmente, nenhum elemento esta na arvore e
	// a menor distancia eh infinito
	for(i,NUMEROVERTICES) {
		arvore[i] = false;
		mindist[i] = INF;
	}
	
	// menor distancia ate o inicio eh zero
	mindist[inicio] = 0.0;
	viz = inicio;
	while (cont < NUMEROVERTICES-1) {
		// coloca o no na arvore
		arvore[viz] = true;
		// ajusta a distancia dos vizinhos do no
		for(i,NUMEROVERTICES) {
			double p = 0.0;
			for(j,NUMOBJETIVOS) p += (lambda[j]*custos[j][viz][i]);
			if (!arvore[i] && i != viz && mindist[i] > p) {
				mindist[i] = p;
				ant[i] = viz;
			}
		}
		viz = -1;
		maisprox = INF;
		
		// procura o no que tem a menor distancia nesta iteracao
		// e que ainda nao foi colocado
		for(i,NUMEROVERTICES) {
			if (!arvore[i] && mindist[i] < maisprox) {
				viz = i;
				maisprox = mindist[i];
			}
		}
		
		// adiciona a aresta entre ant[viz] e viz
		s.edges[cont][0] = ant[viz];
		s.edges[cont][1] = viz;
		cont++;
	}
	// atualiza os custos da solucao
	s.calcularFitness();
}

#undef INF
#undef for
