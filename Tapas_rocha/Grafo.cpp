#include "Grafo.h"
#include <iostream>
#include <stack>
#include "Parametros.h"
using namespace std;

Grafo::Grafo(){};
Grafo::Grafo(int n1){
	n = n1;
	lista_vertices = new Vertice*[n];
	matrixArestas = new Aresta**[n];
	for (int i=0; i<n; i++){
		matrixArestas[i] = new Aresta*[n];
		for (int j=0; j<n; j++){
			matrixArestas[i][j] = NULL;
		}
	}

}

void Grafo::addVertice(int id){
	Vertice *novo = new Vertice(id);
	lista_vertices[id] = novo; /*PADRÃO: A faixa de id's dos vértices é de 0 até n-1*/
}

Vertice *Grafo::getVertice(int id){
	return lista_vertices[id]; 		
}

Aresta *Grafo::addAresta(int id, int origem, int destino, float peso1, float peso2){
	Aresta *nova = new Aresta(id, origem, destino, peso1, peso2);
	//arestasByIndex[id] = nova; // transferido pra Update
	matrixArestas[origem][destino] = nova;
	matrixArestas[destino][origem] = nova;
	lista_vertices[origem]->adicionaAresta(nova);
	lista_vertices[destino]->adicionaAresta(nova);
	return nova;
}

Aresta * Grafo::getArestas(int i){ /*retorna a primeira posição do vetor arestasByIndex*/
	return arestasByIndex[i];
}
int Grafo::getQuantArestas(){
	return arestasByIndex.size();
}
map <int, Aresta *> Grafo::get_allArestas(){
	return arestasByIndex;

}

int Grafo::getQuantVertices(){	
	return n;	
}
Aresta ** Grafo::getAllArestasPtr(){
	Aresta **arestasPtr = new Aresta*[arestasByIndex.size()];
	for (int i=0; i<arestasByIndex.size(); i++){
		arestasPtr[i] = arestasByIndex[i];
	}
	return arestasPtr;
}

int Grafo::getStatus(int i){
	return status[i];
}
void Grafo::setStatus(int i, int status111){
  status[i]=status111;
}
void Grafo::updateIndex(){
	int id = 0;
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			if (matrixArestas[i][j]!=NULL){
				arestasByIndex[id] = matrixArestas[i][j];
				arestasByIndex[id]->setId(id);
				status[id] = 0;
                id++;
			}
		}
	}
}

/*retorna NULL caso a aresta nao exista*/
Aresta *Grafo::getArestas(int origem, int destino){
  if (origem<destino) return matrixArestas[origem][destino];
  else return matrixArestas[destino][origem];
}


// codigo adptado de Sourd and Spaanjard(2008)
void Grafo::computaPreSorting(){
  Aresta **corrente = getAllArestasPtr();
  mergesort(0, 0, 0, 0, corrente, getQuantArestas(),1);
  float lambda = 0;
  arestasEscalarizadas.push_back(make_pair(1, corrente));
  for (;;){
    Aresta **arestasPtr = new Aresta*[getQuantArestas()];
    float nextLambda = 1;
    int j = -1;
    for (int i=0; i<getQuantArestas()-1; i++){
      arestasPtr[i] = corrente[i];
      Aresta *e = corrente[i];
      Aresta *f = corrente[i+1];
      if (e->getPeso2()>f->getPeso2()){
        float next = (e->getPeso1()-f->getPeso1())/((float) e->getPeso1()-e->getPeso2() + f->getPeso2()-f->getPeso1());
        if (next < nextLambda){
          nextLambda = next;
          j = i;
        }
      }
    }
    arestasPtr[getQuantArestas()-1] = corrente[getQuantArestas()-1];
    if (j==-1){
      delete[] arestasPtr; 
      break;
    }
    Aresta *aux = arestasPtr[j]; 
    arestasPtr[j] = arestasPtr[j+1];
    arestasPtr[j+1] = aux;
    corrente = arestasPtr;
    if (nextLambda > lambda){
      arestasEscalarizadas.push_back(make_pair(1-nextLambda, corrente));
      //if (arestasEscalarizadas.size()==maxVetoroes) break;
    }
    lambda = nextLambda;
  }
}

vector< pair<float, Aresta** > > Grafo::getPreSorting(){
  return arestasEscalarizadas;
}
