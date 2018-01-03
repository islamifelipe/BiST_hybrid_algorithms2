#ifndef RMC_PRIM
#define RMC_PRIM

#include "param.h"
#include "SolucaoEdgeSet.cpp"

#include <iostream>
using namespace std;

/*This code file was kindly provided by Monteiro */


#define INF 1e9
#define PER_ACCEPTED 0.03 
#define for(i,n) for (int i=0;i<n;i++)


/*Procedimento rmc-prim normal ==> que gera uma árvore total */
void rmcPrim (SolucaoEdgeSet &s, double lambda[NUMOBJETIVOS]) {
	//cout<<"============== rm1 ============== "<<endl;
	bool arvore[NUMEROVERTICES]; // elementos ja colocados na arvore
	double mindist[NUMEROVERTICES]; // menor distancia ate cada no
	int cont = 0; // numero de elementos ja colocados
	int viz, ant[NUMEROVERTICES];
	double maisprox;
	double maisdist;
	int lrc [NUMEROVERTICES];
	int lrctam = 0;

	int inicio = IRandom(0,NUMEROVERTICES-1);

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
		maisdist = 0.0;
		lrctam = 0;

		// coloca os que nao tem distancia inf na lrc. o tamanho da lrc sera regulado dps de acordo com o custo
		for(i,NUMEROVERTICES) {
			if (!arvore[i] && mindist[i] < INF) {
				lrc [lrctam] = i;
				lrctam++;
				if (mindist[i] < maisprox)
					maisprox = mindist[i];
				if (mindist[i] > maisdist)
					maisdist = mindist[i];
			}
		}

		double maxcustoaceito = maisprox + (maisdist-maisprox)*PER_ACCEPTED;

		int auxlrc [NUMEROVERTICES];
		int auxlrctam = 0;
		for(i,lrctam) {
			if (mindist[lrc[i]] > maxcustoaceito) continue;
			else {
				auxlrc [auxlrctam] = lrc[i];
				auxlrctam++;
			}
		}

		viz = auxlrc [IRandom (0, auxlrctam-1)];

		// adiciona a aresta entre ant[viz] e viz
		if ( ant[viz]< viz){
			s.edges[cont][0] = ant[viz];
			s.edges[cont][1] = viz;
		} else {
			s.edges[cont][0] = viz;
			s.edges[cont][1] = ant[viz];
		}		

		cont++;
	}
	// atualiza os custos da solucao
	s.calcularFitness();
}

void rmcPrim (SolucaoEdgeSet &s, double lambda[NUMOBJETIVOS], int tamanho) {
	//cout<<"============== rm2 ============== "<<endl;
	
	bool arvore[NUMEROVERTICES]; // elementos ja colocados na arvore
	double mindist[NUMEROVERTICES]; // menor distancia ate cada no
	int cont = 0; // numero de elementos ja colocados
	int viz, ant[NUMEROVERTICES];
	double maisprox;
	double maisdist;
	int lrc [NUMEROVERTICES];
	int lrctam = 0;

	int inicio = IRandom(0,NUMEROVERTICES-1); // vértice inicial

	// inicialmente, nenhum elemento esta na arvore e
	// a menor distancia eh infinito
	for(i,NUMEROVERTICES) {
		arvore[i] = false;
		mindist[i] = INF;
	}

	// menor distancia ate o inicio eh zero
	mindist[inicio] = 0.0;
	viz = inicio;
	while (cont < tamanho) {
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
		maisdist = 0.0;
		lrctam = 0;

		// coloca os que nao tem distancia inf na lrc. o tamanho da lrc sera regulado dps de acordo com o custo
		for(i,NUMEROVERTICES) {
			if (!arvore[i] && mindist[i] < INF) {
				lrc [lrctam] = i;
				lrctam++;
				if (mindist[i] < maisprox)
					maisprox = mindist[i];
				if (mindist[i] > maisdist)
					maisdist = mindist[i];
			}
		}
		double maxcustoaceito = maisprox + (maisdist-maisprox)*PER_ACCEPTED;

		int auxlrc [NUMEROVERTICES];
		int auxlrctam = 0;
		for(i,lrctam) {
			if (mindist[lrc[i]] > maxcustoaceito) continue;
			else {
				auxlrc [auxlrctam] = lrc[i];
				auxlrctam++;
			}
		}

		viz = auxlrc [IRandom (0, auxlrctam-1)];

		// adiciona a aresta entre ant[viz] e viz
		
		if ( ant[viz]< viz){
			s.edges[cont][0] = ant[viz];
			s.edges[cont][1] = viz;
		} else {
			s.edges[cont][0] = viz;
			s.edges[cont][1] = ant[viz];
		}
		cont++;
	}
	// atualiza os custos da solucao
	s.calcularFitness(tamanho);
}

// TODO: falta o terceiro procedimento rmcprim

#undef INF
#undef for

#endif
