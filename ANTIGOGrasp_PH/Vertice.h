#include <vector>
#include "Aresta.h"
using namespace std;
 
class Vertice{
	private: 
		int id; /*identificador do vértice*/
		vector <Aresta*> adjacentes; /*é um vetor (de ponteiros) de arestas que saem do vértice*/
		
	public:
		Vertice (int id2); 
		
		Vertice();
		
		int getId();
		
		void adicionaAresta(Aresta *a);
		
		Aresta* getAresta(int i);
		void removeAresta(Aresta *a);
		
		int getGrau();
		
};
