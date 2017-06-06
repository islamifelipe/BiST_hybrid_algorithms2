/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements a hybrid Corley's (1985) and Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
#=======================================================================
*/



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
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
#include "margeSort.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;

struct tms tempsInit, tempsFinal; // para medir o tempo

int n, nA;

//map<int, int* > *X = new map<int, int* >[n+1];
map<int, int* > *X;
	/* X é um vetor de map's. 
	 * cada índice do vetor (0...n), representa o valor das iteracoes r do algoritmo do Corley
	 * cada item o map é um valor de k = 1 ... mr
	 * O tamanho de X nao aumenta. Somente aumenta o tamanho do map
     * dado um id do map com m elementos, busca é feita em O(mlogm)
     * O valor do map é um vator de **** n **** inteiros que representam os vértices 
     incluidos na arvore. Se um indice i for 1, entao o vertice 1 está na arvore  
	 */

//map<int, int* > *A = new map<int, int* >[n+1];
map<int, int* > *A;
	 /* Semelhante ao vetor X, mas aqui o int* é um vetor de nA inteiros 
	  * se um indice i do int* for 1, entao a i-esima aresta (0 à nA-1) está presente na arvore   	
	  */

//int m[n+1]; // de 1 à n : nao utilisar o 0
int *m;
map <int, Aresta *> arestasPtr;

bool a_domina_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && (a->getPeso1() < b->getPeso1() || a->getPeso2() < b->getPeso2())){
		return true;
	} else return false;
}

bool a_domina_fracamente_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2()){
		return true;
	} else return false;
}

int* vmin(int* vertices, Grafo &my_grafo){
	map <int, Aresta *> arestas = my_grafo.get_allArestas();
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
				if (retorno[j] == 1 && retorno[i] == 1){
					if (a_domina_b(ai, arestas[j])){
						retorno[j] = 0;
					} else if (a_domina_b(arestas[j], ai)){
						retorno[i] = 0;
					}
				}
			}
		}
	}
	return retorno;
}

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

bool t1_domina_t2(int *t1, int *t2, map <int, Aresta *> arestas){
	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
	for (int i=0; i<arestas.size(); i++){
		if (t1[i]==1){
			t1_peso1+=arestas[i]->getPeso1();
			t1_peso2+=arestas[i]->getPeso2();
		} 
		if (t2[i]==1){
			t2_peso1+=arestas[i]->getPeso1();
			t2_peso2+=arestas[i]->getPeso2();
		}
	}
	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
		return true;
	} else return false;
}



void printResultado(){
	cout<<m[n]<<endl;
   	cout<<endl;
    for (int k = 1; k <= m[n]; k++){ // cada arvore formada
    	int *arestas  = A[n][k]; 
    	float cont1 = 0, cont2 = 0;
    	cout<<"Arvore "<<k<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
			if (arestas[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")\n"<<endl;
    	cout<<endl;
    }
}


void *tempo(void *nnnn){
	while (true){
		times(&tempsFinal);   /* current time */ // clock final
		clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
		float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
		
		if (sec>1000000){ // se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
			cout<<sec<<endl;
			cout<<"TEMPO LIMITE ATINGIDO..."<<endl;

			printResultado();
			//cout<<"saindo... valor de ppp="<<ppp<<endl;
			exit(-1);
		}
	}
}

int main(){

		
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
	//nA = id; // quantidade de arestas do grafo
	

	times(&tempsInit);  // pega o tempo do clock inical

	// para medir o tempo em caso limite
	pthread_t thread_time; 
	pthread_attr_t attr;
	int nnnnnnnn=0;
	if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ // on criee efectivement la thread de rechaufage
        printf("Error to create the thread");
        exit(-1);
    }
    //

	my_grafo.excluiProibidas(); // primeiro, excluimos as proibidas
	my_grafo.updateIndex(); // depois, atualizamos os idexes das arestas no map
	my_grafo.marcaObrigatorias(); // determinanmos as obrigatorias
	nA = my_grafo.getQuantArestas();
	

	m = new int[n+1];	
	for (int mmm=0; mmm<=n; mmm++)m[mmm] = 0; // by felipe
	 m[1] = 1;

	 A = new map<int, int* >[n+1];
	 X = new map<int, int* >[n+1];
	 arestasPtr = my_grafo.get_allArestas();

     X[1][1] = new int[n];
     for (int mmm=0; mmm<n; mmm++)X[1][1][mmm] = 0; // by felipe
     X[1][1][0] = 1; 
     /* primeiro inidce r = 1 
        segundo indice k = 1
        terceiro indice vertice 0 incluso
     */ 

    A[1][1] = new int[nA];
    for (int mmm=0; mmm<nA; mmm++)A[1][1][mmm] = 0; // by felipe

    for (int r = 1; r<n; r++){
    	map<int, int* > Wr; 
    	m[r+1] = 0;
    	/* pra guardar o retorno do Step2
	     O indice do map é o valor de K (que será reinicializado a cada execucao do passo 2)
	     o int* é o conjunto de id de arestas 
	     */

    	for (int k = 1; k<=m[r]; k++){
    		Wr[k] = vmin(X[r][k], my_grafo);  //Step2
    	}
	     
	     for (int s = 1; s<=m[r]; s++){

	     	int *wrs = Wr[s];
	     	//int *wrs = new int[]
	     	map <int, Aresta *> arestas = my_grafo.get_allArestas();
	 
	     	for (int j = 0; j<nA; j++){
	     		if (wrs[j]==1){ // para escolher uma aresta em wrs
					wrs[j]=0;
					m[r+1]+=1;
					//cout<<"s = "<<s<<"  r = "<<r<<" m[r+1] = "<<m[r+1]<<endl;
		
					X[r+1][m[r+1]] = new int[n];
					for (int y = 0; y<n; y++){
					 X[r+1][m[r+1]][y] = X[r][s][y]; // apenas para fazer uma copia // pode MELHORAR
					}
					A[r+1][m[r+1]] = new int[nA];
					for (int y = 0; y<nA; y++){
					 A[r+1][m[r+1]][y] = A[r][s][y]; // apenas para fazer uma copia // pode MELHORAR
					}
					int p = arestas[j]->getOrigem();
					int q = arestas[j]->getDestino();
					X[r+1][m[r+1]][p] = 1; // redundante
					X[r+1][m[r+1]][q] = 1;
					A[r+1][m[r+1]][j] = 1;
					if (m[r+1] != 1){ //precisa incluir um teste de dominância nos testes em k do passo 6
						bool l = false;
						for (int k = 1; k<=m[r+1]-1; k++){
							if (isEgal(A[r+1][m[r+1]], A[r+1][k], nA)){
								m[r+1]-=1;
								l = true;
								break;
							} else if (r+1 == n && t1_domina_t2(A[r+1][k],A[r+1][m[r+1]], arestas)){
								m[r+1]-=1;
								l = true;
								break;
							}
						}

						for (int k = 1; k<=m[r+1]-1 && l == false && r+1 == n; k++){
						 	if (t1_domina_t2(A[r+1][m[r+1]],A[r+1][k], arestas)){
								for (int mmm=k; mmm<m[r+1]; mmm++){
									A[r+1][mmm] = A[r+1][mmm+1];
									X[r+1][mmm] = X[r+1][mmm+1];
								}
								m[r+1]-=1;
								k--;
							}
						}
					}
	     		}
	     	}
	    }
    }
    
    /* OUTPUT : 
    	Clokcs
    	Time (in seconds)
    	number of solutions 

    	the trees
    	cost
    */

    times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<user_time<<endl;
	cout<<(float) user_time / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	
	printResultado();

	return 0;
}
