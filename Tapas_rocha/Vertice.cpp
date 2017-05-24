#ifndef _VERTICE_H_
#define _VERTICE_H_


#include "Vertice.h"
#include "Aresta.h"


Vertice::Vertice (int id2){ // n = número de vértices do grafo
	id = id2;
}
Vertice::Vertice(){};
		
int Vertice::getId(){
	return id;
}
void Vertice::adicionaAresta(Aresta *a){
	adjacentes.push_back(a);			
}
		
Aresta* Vertice::getAresta(int i){ /*retorna um ponteiro para a i-ésima aresta insidente no vértice*/
	return adjacentes[i];		
}
void Vertice::removeAresta(Aresta *a){
	int id = -1;
	for (int i=0; i<adjacentes.size(); i++){
		if (a->getId() == adjacentes[i]->getId()) id = i;
	}
	if (id!=-1) adjacentes.erase(adjacentes.begin()+id);
}
		
int Vertice::getGrau(){ 
	return adjacentes.size();
}

#endif