

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <vector>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include <queue>          // std::queue
#include <stdlib.h>     /* srand, rand */
#include "Grafo.h"

#include <sys/times.h>
#include <unistd.h>

#include <stdio.h> 

using namespace std;

/*	 Deve receber dois arquivos como parâmetro. 
	argv[1] arquivo em que será guardado a fronteira de pareto
	argv[2] arquivo em que será guardado tempo
*/
int main(int argc, const char * argv[]) {

	
	int n;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	Grafo my_grafo(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}
	int nA = id; // quantidade de arestas do grafo	
	int quantidadeObrigatorias = 0;
	pair<int*, pair<float, float> > obrigatorias;
	my_grafo.excluiProibidas(); // primeiro, excluimos as proibidas
	my_grafo.updateIndex(); // depois, atualizamos os idexes das arestas no map
	my_grafo.marcaObrigatorias(quantidadeObrigatorias); // determinanmos as obrigatorias
	nA= my_grafo.getQuantArestas();
	for (int i=0; i<nA; i++){
		Aresta *a = my_grafo.getArestas(i);
		cout<<a->getOrigem()<<" "<<a->getDestino()<<" "<<a->getPeso1()<<" "<<a->getPeso2();
		if (my_grafo.getStatus(i)==0)cout<<" --> Opcional"<<endl;
		else if (my_grafo.getStatus(i)==1) cout<<" --> Obrigatoria"<<endl;
	}

	// se uma arvore é pareto otima, entao ela tem todas as obrigaotias

	return 0;
}