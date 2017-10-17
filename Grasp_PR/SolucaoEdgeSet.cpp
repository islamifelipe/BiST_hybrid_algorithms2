#ifndef SOLUCAOZG_CPP
#define SOLUCAOZG_CPP
/*This code file was kindly and partially provided by Monteiro */

#include <cstdio>
#include <queue>
#include "Solucao.cpp"
#include "UnionFind.cpp"
#include "param.h"
#include "auxEdgeStruct.h"
#include <algorithm> // std::sort
#include <iostream>
#include <utility>      // std::pair
#include <climits>      // std::pair

using namespace std;

extern double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
extern int contCrossovers; // quantidade de crossovers executados
extern int contMutacoes; // quantidade de mutaçoes executadas
extern int contQuantCalculouFitness; // guarda a quantidade de vezes em que a funçao calculaOwa foi invocada


typedef struct {
	int listaadj[NUMEROVERTICES][NUMEROVERTICES], graus[NUMEROVERTICES];
	bool completo;
} grafo;


class SolucaoEdgeSet : public Solucao {
	public:
	int edges[NUMEROVERTICES-1][2]; // para cada linha (aresta da arvore), teremos duas colunas (vertices extremos i e j)
	int nEdges;
	UnionFind uf;
	grafo *g;
	

	SolucaoEdgeSet(int n,TRandomMersenne &r) {
		
		nEdges = n;
		for (int i=0; i<NUMOBJETIVOS; i++){
			f[i]=0;
		}
		rg = &r;
		g = NULL;
	}
	~SolucaoEdgeSet() {

	}

    
    // util para guardar copias de solucoes somente para comparacao
    void shallowCopy(SolucaoEdgeSet &s) {
        for (int i=0; i<NUMOBJETIVOS; i++){
			s.f[i] = f[i];
		}
    }

    void operator=(SolucaoEdgeSet &s) {
		nEdges = s.nEdges;
		for (int i=0; i<NUMOBJETIVOS; i++){
			f[i] = s.f[i];
		}
		memcpy(edges,s.edges,sizeof(edges));
	}

    void calcularObjetivos() {
		for (int j=0;j<NUMOBJETIVOS;j++){
			f[j] = 0.0;
		}
		for (int i=0;i<NUMEROVERTICES-1;i++)
			for (int j=0;j<NUMOBJETIVOS;j++)
				f[j] += f(j,edges[i][0],edges[i][1]);
	}

	// para grafos completos
    void doRandomWalk() {

        int vertlist [NUMEROVERTICES];
        for (int i = 0; i < NUMEROVERTICES; i++)
            vertlist[i]=i;
        int vertctr = NUMEROVERTICES-1; //tamanho valido da lista;

        int v_ind = rg->IRandom(0,vertctr);
        int v_esc = vertlist[v_ind];
        vertlist [v_ind] = vertlist[vertctr];
        vertlist [vertctr] = v_esc;
        vertctr--;
        int viz1, viz2, viz2_ind;
        int cont = 0;
        while (cont < NUMEROVERTICES-1) {

            viz1 = vertlist [rg->IRandom(vertctr+1, NUMEROVERTICES-1)]; // sorteia algum vértice ja na arvore
            viz2_ind = rg->IRandom(0,vertctr);// sorteia um fora
            viz2 = vertlist[viz2_ind];

            vertlist [viz2_ind] = vertlist[vertctr];
            vertlist [vertctr] = viz2;
            vertctr--;
            if (viz1<viz2) {
	            edges[cont][0] = viz1;
	            edges[cont][1] = viz2;
	        } else {
	        	edges[cont][0] = viz2;
	            edges[cont][1] = viz1;
	        }
            cont++;
        }
        calcularObjetivos();
        //assert (confereArestas());
    }


    // para grafos completos e incompletos 
    void RandomWalk(bool unionGraph[NUMEROVERTICES][NUMEROVERTICES]){
    	uf.clear();
    	std::vector<pair<int, int> > amostral;
    	for (int i=0; i<NUMEROVERTICES; i++){
    		for (int j=i+1; j<NUMEROVERTICES; j++){
    			if(unionGraph[i][j]==true) {
    				amostral.push_back(make_pair(i,j));
    			}
    		}
    	}
    	int cont=0;
    	while (cont<NUMEROVERTICES-1 && amostral.size()>0){
			int are = rg->IRandom(0,amostral.size()-1);
			if (uf.sameClass(amostral[are].first,amostral[are].second)==false){
				uf.unionClass(amostral[are].first,amostral[are].second);
				if (amostral[are].first<amostral[are].second){
					edges[cont][0] = amostral[are].first;
					edges[cont][1] = amostral[are].second;
				} else {
					edges[cont][0] = amostral[are].second;
					edges[cont][1] = amostral[are].first;
				}
				cont++;
			}
			amostral.erase(amostral.begin()+are);
		}
		if (cont<NUMEROVERTICES-1) cout<<"ERROR RandomWalk cont = "<<cont<<" amostral.size() = "<<amostral.size()<<endl;
    }

 //    /* Faz o crossover entre dois individuos.
 //    BAseado no crossover sugerido por Raidl and Julstrom (2003)*/
	// void crossover(const SolucaoEdgeSet &pai, const SolucaoEdgeSet &mae) {
	// 	contCrossovers++;
	// 	bool unionGraph[NUMEROVERTICES][NUMEROVERTICES];
	// 	memset(unionGraph,false,sizeof(unionGraph));

	// 	for (int i=0;i<NUMEROVERTICES-1;i++) {
	// 		if (!unionGraph[pai.edges[i][0]][pai.edges[i][1]]) {
	// 			unionGraph[pai.edges[i][0]][pai.edges[i][1]] = unionGraph[pai.edges[i][1]][pai.edges[i][0]] = true;
	// 		}
	// 	}
	// 	for (int i=0;i<NUMEROVERTICES-1;i++) {
	// 		if (!unionGraph[mae.edges[i][0]][mae.edges[i][1]]) {
	// 			unionGraph[mae.edges[i][0]][mae.edges[i][1]] = unionGraph[mae.edges[i][1]][mae.edges[i][0]] = true;
	// 		}
	// 	}
	// 	RandomWalk(unionGraph);
	// 	calcularObjetivos();
	// }


	void printSolucao() {
		for (int i=0;i<NUMEROVERTICES-1;i++){
			cout<<edges[i][0]<<" "<<edges[i][1]<<" ";
			for (int o=0; o<NUMOBJETIVOS; o++){
				cout<<f(o,edges[i][0],edges[i][1])<<" ";
			}
			cout<<endl;	
		}
		cout<<"( ";
		for (int o=0; o<NUMOBJETIVOS; o++){
			cout<<this->f[o]<<" ";
		}
		cout<<")"<<endl;
	}

	


	/* Faz a troca das arestas ai e aj, religando no formato 2-OPT
	 * Complexidade O(1) */
	void trocaArestas(int ai, int aj, char tipo, SolucaoEdgeSet &soloriginal) {
		int a = soloriginal.edges[ai][0];
		int b = soloriginal.edges[ai][1];
		int c = soloriginal.edges[aj][0];
		int d = soloriginal.edges[aj][1];

		//inicialmente temos a-b e c-d

		int novaA[2]={0,0}, novaB[2]={0,0};
		novaA[0] = a;
		novaB[0] = b;

		// se todos ficarem na mesma componente, junta AC/BD, se nao junta AD/BC
		if (tipo == 0) {
			// faz a uniao AC e BD
			novaA[1] = c;
			novaB[1] = d;
		} else if (tipo == 1){
			// faz a uniao AD e BC
			novaA[1] = d;
			novaB[1] = c;
		}
		if (novaA[0]<novaA[1]){
			edges[ai][0] = novaA[0];
			edges[ai][1] = novaA[1];
		} else {
			edges[ai][0] = novaA[1];
			edges[ai][1] = novaA[0];
		}
		if (novaB[0]<novaB[1]){
			edges[aj][0] = novaB[0];
			edges[aj][1] = novaB[1];
		} else {
			edges[aj][0] = novaB[1];
			edges[aj][1] = novaB[0];
		}
	}

	char calcularTrocaArestas(int ai, int aj, SolucaoEdgeSet &soloriginal) {
		// Calcula a possivel troca das arestas "ai" com "aj"
		// Complexidade O(n)
		// uniao busca
		uf.clear();
		for (int k=0;k<NUMOBJETIVOS;k++) f[k] = 0.0; // (re)inicializa os objetivos
		
		for (int i=0;i<NUMEROVERTICES-1;i++)
			if (i != ai && i != aj) {
				edges[i][0] = soloriginal.edges[i][0];
				edges[i][1] = soloriginal.edges[i][1];
				uf.unionClass(edges[i][0],edges[i][1]);
				for (int k=0;k<NUMOBJETIVOS;k++)
					f[k] +=  f(k,edges[i][0],edges[i][1]);
			}
		int a = soloriginal.edges[ai][0];
		int b = soloriginal.edges[ai][1];
		int c = soloriginal.edges[aj][0];
		int d = soloriginal.edges[aj][1];

		char tipoTroca;
		// junta a-c e b-d
		uf.unionClass(a,c);
		uf.unionClass(b,d);
		// se todos ficarem na mesma componente, junta AC/BD, se nao junta AD/BC
		if (uf.sameClass(a,b) && uf.sameClass(a,c) && uf.sameClass(a,d)) {
			// faz a uniao AC e BD
			tipoTroca = 0;
			for (int k=0;k<NUMOBJETIVOS;k++)
				f[k] = f[k] + ( f(k,a,c) + f(k,b,d) );
		} else {
			// faz a uniao AD e BC
			tipoTroca = 1;
			for (int k=0;k<NUMOBJETIVOS;k++)
				f[k] = f[k] + ( f(k,a,d) + f(k,b,c) );
		}
		return tipoTroca;
	}

	

};

#endif
