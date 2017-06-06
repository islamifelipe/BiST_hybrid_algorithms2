
#include "corley.h"
#include <map>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <utility>      // std::pair
using namespace std;

bool a_domina_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && (a->getPeso1() < b->getPeso1() || a->getPeso2() < b->getPeso2())){
		return true;
	} else return false;
}

// bool isEgal(int *t1, int *t2, int size){
// 	for (int i=0; i<size; i++){
// 		if (t1[i]!=t2[i]) return false; // TODO: pode melhorar
// 	}
// 	return true;
// }

bool isEgal(pair<float, float> p1, pair<float, float> p2){
	return p1.first==p2.first && p1.second==p2.second ;
}

int* vmin(int* vertices, Grafo *my_grafo){
	map <int, Aresta *> arestas = my_grafo->get_allArestas();
	int *retorno = new int[arestas.size()]; // vetor dos id das arestas

	// primeiramente, seleciona-se as arestas conforme a regra classica de conjunto dijunto de prim   

	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		Aresta *a = arestas[i];
		if (vertices[a->getOrigem()] != vertices[a->getDestino()]){
			retorno[a->getId()] = 1; 
		} else {
			retorno[a->getId()] = 0; 
		}
	}

	// depois verificamos a dominância entre as arestas

	for (int i = 0; i<arestas.size(); i++){ // O(m^2)
		if (retorno[i] == 1){ // se a aresta de id=i está contida no conjunto... 
			Aresta *ai = arestas[i];
			for (int j = i+1; j<arestas.size(); j++){
				if (retorno[j] == 1 && retorno[i] == 1 && my_grafo->getStatus(i)!=1){
					if (a_domina_b(ai, arestas[j])){
						retorno[j] = 0;
					} else if (a_domina_b(arestas[j], ai)){
						retorno[i] = 0;
					} else{
						float pro = rand()%100; //0 e 99
						if (pro>=45){ // 60 por cento de change se aceitar a aresta e 40 de recusar
							retorno[i] = 0;
						} 
					}
				}
			}
		}
	}
	return retorno;
}
int iteracoes = 0;
/*state tem quantVertices vertices e quantVertices-1 arestas*/
list <pair<int*,  pair<float, float> > > corley(Grafo *g, pair<int*, pair<float, float> > state, int quantVertices){
	/* deveria ser 0...n+1, onde n é a quantidade de arestas de g (e nao de state)
	* mas como state ja possui quantVertices vertices, era como se 'r' (do Corley original) fosse r=quantVertices
	'r' vai de quantVertices (0) até g->getQuantVertices() (g->getQuantVertices()-quantVertices)
	*/
	int size_vector= g->getQuantVertices()-quantVertices+1;
	int *m = new int[size_vector+1]; 
	for (int mmm=0; mmm<=size_vector; mmm++) m[mmm] = 0;
	m[1] = 1;
	map<int, int* > *X = new map<int, int* >[size_vector+1];
	/* X é um vetor de map's. 
	 * cada índice do vetor (0...size_vector), representa o valor das iteracoes r do algoritmo do Corley
	 * cada item o map é um valor de k = 1 ... mr
	 * O tamanho de X nao aumenta. Somente aumenta o tamanho do map
     * O valor do map é um vator de **** n **** inteiros que representam os vértices 
     incluidos na arvore. Se um indice i for 1, entao o vertice 1 está na arvore  
	 */

	 map<int, int* > *A = new map<int, int* >[size_vector+1];
	 map<int, pair<float, float> > *pesos = new map<int, pair<float, float> >[size_vector+1];
	 /* Semelhante ao vetor X, mas aqui o int* é um vetor de nA inteiros 
	  * se um indice i do int* for 1, entao a i-esima aresta (0 à nA-1) está presente na arvore   	
	  */
	  X[1][1] = new int[g->getQuantVertices()];
	  A[1][1] = new int[g->getQuantArestas()];
	  pesos[1][1] = make_pair(0,0); 
	  for (int mmm=0; mmm<g->getQuantVertices(); mmm++) X[1][1][mmm] = 0;
	  for (int mmm=0; mmm<g->getQuantArestas(); mmm++) A[1][1][mmm] = 0;
	  for (int mmm=0; mmm<quantVertices-1; mmm++){ // constroi o X e A inicial a partir das arestas do state 
	  	Aresta *a = g->getArestas(state.first[mmm]);
	  	X[1][1][a->getOrigem()] = 1;
	  	X[1][1][a->getDestino()] = 1;
	  	A[1][1][state.first[mmm]] = 1;
	  	pesos[1][1].first+=a->getPeso1();
	  	pesos[1][1].second+=a->getPeso2();
	  }
	  for (int r = 1; r<size_vector; r++){
	  	map<int, int* > Wr; 
    	m[r+1] = 0;
    	/* pra guardar o retorno do Step2
	     O indice do map é o valor de K (que será reinicializado a cada execucao do passo 2)
	     o int* é o conjunto de id de arestas 
	     */
    	for (int k = 1; k<=m[r]; k++){
    		Wr[k] = vmin(X[r][k], g);  //Step2
    	}

    	for (int s = 1; s<=m[r]; s++){
    		int *wrs = Wr[s];
    		for (int j = 0; j<g->getQuantArestas(); j++){
    			if (wrs[j]==1){ // para escolher uma aresta em wrs
    				iteracoes++;
    				wrs[j]=0;
					m[r+1]+=1;

					X[r+1][m[r+1]] = new int[g->getQuantVertices()];
					for (int y = 0; y<g->getQuantVertices(); y++){
					 X[r+1][m[r+1]][y] = X[r][s][y]; // apenas para fazer uma copia // pode MELHORAR
					}
					A[r+1][m[r+1]] = new int[g->getQuantArestas()];
					for (int y = 0; y<g->getQuantArestas(); y++){
					 A[r+1][m[r+1]][y] = A[r][s][y]; // apenas para fazer uma copia // pode MELHORAR
					}
					pesos[r+1][m[r+1]] = make_pair(pesos[r][s].first ,pesos[r][s].second);
					int p = g->getArestas(j)->getOrigem();
					int q = g->getArestas(j)->getDestino();
					X[r+1][m[r+1]][p] = 1; 
					X[r+1][m[r+1]][q] = 1;
					A[r+1][m[r+1]][j] = 1;
					pesos[r+1][m[r+1]].first +=g->getArestas(j)->getPeso1();
	  				pesos[r+1][m[r+1]].second+=g->getArestas(j)->getPeso2();

					if (m[r+1] != 1){ 
						bool l = false;
						for (int k = 1; k<=m[r+1]-1; k++){
							if (isEgal(pesos[r+1][m[r+1]], pesos[r+1][k])){
								m[r+1]-=1;
								l = true;
								break;
							}
						}
					}
    			}
    		}
    	}
	}

	list <pair<int*,  pair<float, float> > > retorno;
	//cout<<"Quantidade de vizinhos: "<<m[size_vector]<<endl;
   	//cout<<endl;
    for (int k = 1; k <= m[size_vector]; k++){ // cada arvore formada
    	int *arestas  = A[size_vector][k]; 
    	int contarest=0;
    	pair<int*,  pair<float, float> > ops = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
    	//cout<<"Vizinho "<<k-1<<endl;
    	for (int a = 0; a<g->getQuantArestas(); a++){ // cada aresta da arvore
			if (arestas[a] == 1){
				ops.second.first+=g->getArestas(a)->getPeso1();
				ops.second.second+=g->getArestas(a)->getPeso2();
				ops.first[contarest++] = a;
				//cout<<g->getArestas(a)->getOrigem()<<" "<<g->getArestas(a)->getDestino()<<" "<< g->getArestas(a)->getPeso1()<<" "<<g->getArestas(a)->getPeso2()<<endl;
    		}
    	}
    	retorno.push_back(ops);
    	 // cout<<"("<<ops.second.first<<", "<<ops.second.second<<")\n"<<endl;
    	 // cout<<endl;
    }
    for (int iii=0; iii<=size_vector; iii++){
    	for (int jjj=0; jjj<m[iii]; jjj++) {
    		delete[] X[iii][jjj];
    		delete[] A[iii][jjj];
    	}
    }
    delete[] X;
    delete[] A;
    cout<<"Iteracoes = "<<iteracoes<<endl;
    return retorno;
}
