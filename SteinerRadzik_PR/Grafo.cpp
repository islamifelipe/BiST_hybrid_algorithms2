#include "Grafo.h"
#include <iostream>
#include <stack>
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

void Grafo::updateIndex(){
	int id = 0;
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			if (matrixArestas[i][j]!=NULL){
				arestasByIndex[id] = matrixArestas[i][j];
				arestasByIndex[id]->setId(id);
				status[id++] = 0;
			}
		}
	}
}
//Ta bom, poderia implementar o operador <, eu sei. Mas deu preguiça
bool menor(Aresta *a, Aresta *b){
    if (a->getPeso1()<b->getPeso1()) return a->getPeso2() <= b->getPeso2();
    if (a->getPeso1()>b->getPeso1()) return false;
    //assert(a->getPeso1()==b->getPeso1()) 
    if (a->getPeso2()<b->getPeso2()) return true;
    if (a->getPeso2()==b->getPeso2()) return a->getId()<b->getId();
    return false;
}
//idem
bool maior(Aresta *a, Aresta *b){
    if (a->getPeso1()>b->getPeso1()) return a->getPeso2() >= b->getPeso2();
    if (a->getPeso1()<b->getPeso1()) return false;
    //assert(a->getPeso1()==b->getPeso1()) 
    if (a->getPeso2()>b->getPeso2()) return true;
    if (a->getPeso2()==b->getPeso2()) return a->getId()>b->getId();
    return false;
}
// codigo adptado de Sourd and Spaanjard(2008)
void Grafo::excluiProibidas(){
  int removidas = 0;
    bool *verticesVisitados = new bool[n];
    stack<int> pilha;
    for (int origem=0; origem<n; origem++){
      for (int destino=origem+1; destino<n; destino++){
            if (matrixArestas[origem][destino]!=NULL){
            for (int i = 0; i < n; i++) verticesVisitados[i] = false;
            verticesVisitados[origem] = true;
              pilha.push(origem);
              while(pilha.empty()==false){
                int current = pilha.top();
                pilha.pop();
                verticesVisitados[current] = true;
                for (int adj = 0; adj<lista_vertices[current]->getGrau(); adj++){ // para cada adjacente do vertice corrente
                      int prox = lista_vertices[current]->getAresta(adj)->getDestino();
                        if (prox==current) prox = lista_vertices[current]->getAresta(adj)->getOrigem();
                        
                        // este if abaixo: se o prox vertice nao foi visitado
                  //e se a aresta indo de corrente pra prox domina a aresta corrente (origem destino), entao...
                  if (verticesVisitados[prox]==false && menor(matrixArestas[current][prox], matrixArestas[origem][destino])){
                    //este if abaixo: se a aresta que vai de prox a destino (o destino do laço là de cima) exite, entao verifica se ela domina a aresta corrente (origem destino)
                    if (matrixArestas[prox][destino]!=NULL && menor(matrixArestas[prox][destino], matrixArestas[origem][destino])) {
                      removidas++;
                      Aresta *mm = matrixArestas[origem][destino];
                      lista_vertices[origem]->removeAresta(mm);
                      lista_vertices[destino]->removeAresta(mm);
                      matrixArestas[origem][destino] = NULL;
                                matrixArestas[destino][origem] = NULL;
                                //clear stack
                                for (int kgf = 0; pilha.empty()==false; kgf++){
                                    pilha.pop();
                                }
                      break;
                    } else {
                      pilha.push(prox);
                    }
                  }
                }
              }
        }
      }
    }
    delete[] verticesVisitados;
    cout<<removidas<<" arestas removidas"<<endl;

}

// codigo adptado de Sourd and Spaanjard(2008)
void Grafo::marcaObrigatorias(){
  int obrigatorias = 0;
    bool *verticesVisitados = new bool[n];
    stack<int> pilha;
    for (int origem=0; origem<n; origem++){
      for (int destino=origem+1; destino<n; destino++){
        if (matrixArestas[origem][destino]!=NULL){
          for (int i = 0; i < n; i++) verticesVisitados[i] = false;
          verticesVisitados[origem] = true;
            // stack<int> pilha;
            pilha.push(origem);
            while(pilha.empty()==false){
              int current = pilha.top();
              pilha.pop();
              verticesVisitados[current] = true;
              for (int adj = 0; adj<lista_vertices[current]->getGrau(); adj++){ // para cada adjacente do vertice corrente
                int prox = lista_vertices[current]->getAresta(adj)->getDestino()!=current?lista_vertices[current]->getAresta(adj)->getDestino():lista_vertices[current]->getAresta(adj)->getOrigem();
                // este if abaixo: se o prox vertice nao foi visitado
                //e se a aresta indo de corrente pra prox domina a aresta corrente (origem destino), entao...
                if (verticesVisitados[prox]==false && matrixArestas[current][prox]->getId()!=matrixArestas[origem][destino]->getId() && !maior(matrixArestas[current][prox], matrixArestas[origem][destino])){
                  //este if abaixo: se a aresta que vai de prox a destino (o destino do laço là de cima) exite, entao verifica se ela domina a aresta corrente (origem destino)
                  if (matrixArestas[prox][destino]!=NULL && !maior(matrixArestas[prox][destino], matrixArestas[origem][destino])){
                    verticesVisitados[destino] = true;
                    break;
                  } else {
                    pilha.push(prox);
                     //break;
                  }
                }
              }
            }
            if (verticesVisitados[destino]==false && status[matrixArestas[origem][destino]->getId()] == 0){
              //cout<<origem<<" "<<destino<<" "<<matrixArestas[origem][destino]->getPeso1()<<" "<<matrixArestas[origem][destino]->getPeso2()<<endl;
              obrigatorias++;
              status[matrixArestas[origem][destino]->getId()] = 1;
            } 
        } //end if (matrixArestas[origem][destino]!=NULL){
      }
    }
    delete[] verticesVisitados;
    cout<<obrigatorias<<" obrigatorias"<<endl;

}

