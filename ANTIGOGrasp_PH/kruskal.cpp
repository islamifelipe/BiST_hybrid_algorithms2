#include "Conjunto.h"
#include "kruskal.h"
#include "Grafo.h"
#include "margeSort.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>  
using namespace std;

#define PRECISAO 0.00001



bool kruskal (Grafo *g, Aresta **listaAresta,  int  *A, float &x, float &y){
	
	Conjunto conjunto(g->getQuantVertices());
	int cont=0; 
	int i=0;
	x=0;y=0;
	while (cont<g->getQuantVertices()-1 && i<g->getQuantArestas()){ 
		if (!conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			//A[listaAresta[i]->getId()] = 1;
			A[cont++] = listaAresta[i]->getId();
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			x+=listaAresta[i]->getPeso1();
			y+=listaAresta[i]->getPeso2();
		}
		i++;
	}
	if (cont==g->getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
}