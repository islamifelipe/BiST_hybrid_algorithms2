#ifndef _GRAFO_H_
#define _GRAFO_H_

#include "Vertice.h"
#include <map>
#include <vector>

class Grafo{
	
	private:
		int n,m;
		Vertice **lista_vertices; /*Lista (fixa) de vértice. É um vetor de ponteiros. Cada vértice possui uma lista de arestas adjacentes.*/
		Aresta ** arestasByIndex; /*lista de todas as arestas do Grafo.*/
		int *status; // maximo 499500
		// se 0, é opcional
		// se 1, é obrigatoria

		Aresta *** matrixArestas; // matrixArestas[i][j] = ponteiro para objeto aresta
		Aresta **arestasPtr;
	public:
		Grafo(int n1);
		Grafo();
		void addVertice(int id); // lista_vertices[id] = vertice de identificador id (entre 0 e n-1)
		Vertice *getVertice(int id);
		Aresta *addAresta(int id, int origem, int destino, float peso1, float peso2);
		Aresta * getArestas(int i);
		map <int, Aresta *> get_allArestas();
		Aresta ** getAllArestasPtr();
		void gerarArestasPtr();
		int getQuantArestas();
		int getQuantVertices();
		int getStatus(int i);
		void updateIndex(); // deve ser chamada depois de chamar o método de excluir proibidas. 
		//O método updateIndex pegas as arestas obrigatorias ou opcionais e reajusta os indices
		void excluiProibidas();
		pair<int*, pair<float, float> > marcaObrigatorias(int &obrigatorias2);
		void setN(int n1);
		void setStatus(int i, int status111);
};

#endif