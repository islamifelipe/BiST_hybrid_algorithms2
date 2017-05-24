#ifndef _GRAFO_H_
#define _GRAFO_H_

#include "Vertice.h"
#include "margeSort.h"
#include <map>
#include <vector>
#include <utility>      // std::pair

class Grafo{
	
	private:
		int n;
		Vertice **lista_vertices; /*Lista (fixa) de vértice. É um vetor de ponteiros. Cada vértice possui uma lista de arestas adjacentes.*/
		map <int, Aresta *> arestasByIndex; /*lista de todas as arestas do Grafo.*/
		map <int, int> status;
		// se 0, é opcional
		// se 1, é obrigatoria
		// nao há aresta proibida (todas sao excluida a priori)
		vector< pair<float, Aresta** > > arestasEscalarizadas; //estrutura utilizada para as m^2 possiveis ordenacoes das arestas escalaizadas
		// o float é o lambda entre 0 e 1 (inclusive). O peso é computado como lambda*peso1 + (1 - lambda)*peso2
		Aresta *** matrixArestas; // matrixArestas[i][j] = ponteiro para objeto aresta
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
		void setStatus(int i, int status111);
		void updateIndex(); // deve ser chamada depois de chamar o método de excluir proibidas. 
		//O método updateIndex pegas as arestas obrigatorias ou opcionais e reajusta os indices
		void excluiProibidas();
		void marcaObrigatorias();
		void computaPreSorting();
		Aresta *getArestas(int origem, int destino);
		vector< pair<float, Aresta** > >getPreSorting();

};

#endif