/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements a hybrid Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The second phase uses peth-relinking algorithm
#=======================================================================
*/

/* Pre-processamento do SS
 * Primeira fase do SR
 * Segunda fase:
 *		para cada triângulo retângulo formado por solucoes adjacentes p e q (podemos começar o PR por qualquer uma das duas)
 *			mantém uma lista de solucoes nao suportadas dentro deste triângulo (uma lista para cada triângulo)
 *			guarda os vizinhos (possiveis solucoes) de q que estao fora da zona de exclusao (vizinhos fora do triângulo sao desconsiderados) e atualiza a zona de exclusao (ou seja, retirando da lista possiveis solucoes que se tornem dominadas)
 *				para cada um destes vizinhos, considerar para o PR somente aqueles que se aproximam de p
 *					pega o que minimiza do custo escalarizado para ser o start da proxima iteracao
 *	
 *
 *
*/

 /* MOSTRAR NO EXMEPLO GRAFICO DA DIFERENÇA COM O K_BEST
 		considere o triângulo pq inicial (vazio)
 		considere duas solucoes a e b, onde b domina a.
 		pelo k-best, b seria encontrada primeiro, definiria uma regiao viavel e 'a' nao entraria.
 		no nosso algorimo, 'a' pode ser encontrada primeiro. Logo, precisamos atualizar a zona de exclusao apos uma solucao ser inserida e remover eventuais solucoes encontradas anteriormente e que sao dominada pela solucao inserida agora
 */

 /*
	IMPORTANTE: dizer que a vantagem de calcular as solucoes suportadas previamente é que pode-se delimitar bem o espaço de busca
 	IMPORTANTE: dizer que o PR nos dois sentidos é importante, sobretudo para instâncias dificies, como as anticorrelated
 	dizer também que, aqui, a diversificacao fica por conta da busca em cada triângulo
 */

/*
	PROVAR QUE SE a E b SAO DE TRIANGULOS RETÂNGULOS DIFERENTES, ENTAO a E b SAO INCOMPARAVEIS (prova visual)
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
#include <stdio.h>
using namespace std;

// TODO : ver instâncias concave

#define maxSizePath 5
#define maxVizinhos 4
#define maxSuportadas 70
#define maxSearch 10

typedef struct { // pra calcular as solucoes suportadas
	pair<int*, pair<float, float> > s1; 
	pair<int*, pair<float, float> > s2; 
} ItemFila;


/*Esta funcao é adaptada do algoritmo propsoto na dissertacao de Monteiro (2010)
O procedimento usa uma filha pra obter solucoes suportadas bem distribuidas na fronteira de pareto
O procedimento termina ou quando o limite de iteracoes é atingido ou quando acaba as solucoes suportadas*/
list <pair<int*, pair<float, float> > >  suportadas(Grafo *g){
	list <pair<int*, pair<float, float> > > setSolucoes;
	Aresta **arestasPtr = g->getAllArestasPtr();
	queue <ItemFila> fila;
	int *s1 = new int[g->getQuantVertices()-1]; // primeiro objetivo
	float x=0, y=0; // objetivos
	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),1);
	kruskal(g, arestasPtr,s1,x, y); // arvore para o primeiro objetivo
	pair<int*, pair<float, float> > ps1 = make_pair(s1, make_pair(x,y));
	setSolucoes.push_back(ps1);
	int* s2 = new int[g->getQuantVertices()-1];
	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),2);
	x=0, y=0;
	kruskal(g, arestasPtr, s2,x, y); // arvore para o segundo objetivo
	pair<int*, pair<float, float> > ps2 = make_pair(s2, make_pair(x,y));
	setSolucoes.push_back(ps2);
	fila.push((ItemFila){ps1, ps2});

	int cont=2; 
	while (fila.size()!=0){
		ItemFila item = fila.front();
		fila.pop();
		pair<int*, pair<float, float> > s1 = item.s1;
		pair<int*, pair<float, float> > s2 = item.s2;


		float xl = s1.second.first;
		float yl = s1.second.second;
		float xll = s2.second.first;
		float yll = s2.second.second;

		int *A2 = new int[g->getQuantVertices()-1];
		mergesort(xl, yl, xll, yll, arestasPtr, g->getQuantArestas(),3);
		x=0; y=0;
		kruskal(g, arestasPtr,A2,x, y);

		if (x!=xl && x!=xll && y!=yl && y!=yll){
			pair<int*, pair<float, float> > novo = make_pair(A2, make_pair(x, y));
				
			if (cont<maxSuportadas){
				setSolucoes.push_back(novo);
				cont++;
			}
			fila.push((ItemFila){s1, novo});
			fila.push((ItemFila){novo, s2});
		}

	}
	return setSolucoes;
}

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;
float yp, yq, xp, xq;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinalBuscaLocal ; // para medir o tempo

bool isEgalObjetive(float t1_peso1, float t1_peso2, float t2_peso1, float t2_peso2){
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);
}

list <pair<int*,  pair<float, float> > > vizinhos2(Grafo *g, pair<int*, pair<float, float> > sol, float cateto_x, float cateto_y, float escalarX, float escalarY){
	list <pair<int*,  pair<float, float> > > retorno; /*possiveis candidatos*/
	int custo = sol.second.first*escalarX + sol.second.second*escalarY; // int mesmo
	for (int viz=0; viz<maxVizinhos && custo!=0; viz++){
		float randdd = rand()%custo;
		int idEscolhidaSair=0;
		int soma = 0;
		Conjunto conjunto(g->getQuantVertices());
		for (int aa = 0; aa<g->getQuantVertices()-1; aa++){
			int custoAresta = g->getArestas(sol.first[aa])->getPeso1()*escalarX + g->getArestas(sol.first[aa])->getPeso2()*escalarY;
			if (randdd>=soma && randdd<custoAresta+soma){ // aresta foi escolhida
				idEscolhidaSair = sol.first[aa];
			} else { // aresta fica, ou seja, nao foi escolhida pra sair
				conjunto.union1(g->getArestas(sol.first[aa])->getOrigem(), g->getArestas(sol.first[aa])->getDestino());	
			}
			soma+=custoAresta;
		}
		if (g->getStatus(idEscolhidaSair)==0){
			
			int peso1base = sol.second.first  - g->getArestas(idEscolhidaSair)->getPeso1();
			int peso2base = sol.second.second - g->getArestas(idEscolhidaSair)->getPeso2();
			///cout<<"Sai aresta id="<<idEscolhidaSair<<" Peso1 = "<<g->getArestas(idEscolhidaSair)->getPeso1()<<" Peso2 = "<<g->getArestas(idEscolhidaSair)->getPeso2()<<endl;
			vector<int> arestasPossiveis;
			for (int i = 0; i <g->getQuantArestas(); i++){
		 		if (sol.first[i]!=idEscolhidaSair){
		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
		 				int newPeso1 = peso1base+g->getArestas(i)->getPeso1();
		 				int newPeso2 = peso2base+g->getArestas(i)->getPeso2();
		 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
		 					if (newPeso1<cateto_x && newPeso2<cateto_y){ // garante que o vizinho esteja dentro do triângulo retângulo 
		 						arestasPossiveis.push_back(i);
		 					}
		 				}
		 			}
		 		}
		 	}
		 	if (arestasPossiveis.size()>0){
		 		int idEscolhidaEntra = arestasPossiveis[rand()%arestasPossiveis.size()];
		 		int *base = new int[g->getQuantVertices()-1];
				for (int are = 0; are <g->getQuantVertices()-1; are++){
				 	if (sol.first[are] == idEscolhidaSair) base[are] = idEscolhidaEntra;
				 	else base[are] = sol.first[are];
				} 	
				retorno.push_back(make_pair(base, make_pair(peso1base+g->getArestas(idEscolhidaEntra)->getPeso1(),peso2base+g->getArestas(idEscolhidaEntra)->getPeso2())));
		 	}
		} //else fora
	}
	return retorno;
}

/*Quantidade de arestas que existem em xi (origem), mas que nao existem em xt(target). 
Ou seja, a quantidade de arestas que restam à xi para chegar até xt*/
int distance(Grafo *g, pair<int*, pair<float, float> > s1, pair<int*, pair<float, float> > s2){
	
	return sqrt((s1.second.first - s2.second.first)*(s1.second.first - s2.second.first) + (s1.second.second - s2.second.second)*(s1.second.second - s2.second.second));
}

// void path_relinking(Grafo *g, pair<int*, pair<float, float> > x_startaux, pair<int*, pair<float, float> > x_target, list <pair<int*, pair<float, float> > > &solucoes, float cateto_x, float cateto_y, float escalarX, float escalarY){
// 	pair<int*, pair<float, float> > x_start = make_pair(new int[g->getQuantVertices()-1], make_pair(x_startaux.second.first, x_startaux.second.second));
// 	for (int i=0; i<g->getQuantVertices()-1; i++) x_start.first[i] = x_startaux.first[i];
// 	float delta;
// 	int cont = 0;
// 	delta = distance(g, x_start, x_target);
// 	do {
// 		//delta = distance(g, x_start, x_target); 
// 		if (delta>=1 && cont<maxSizePath ){ //&& solucoes.size()<maxSolPorTriangulo){
// 			list <pair<int*,  pair<float, float> > > viz = vizinhos2(g, x_start,cateto_x, cateto_y, escalarX,escalarY); // somente os vizinhos dentro do triangulo formado pelos catetos x,y
// 			vector <pair <int , pair<int*, pair<float, float> > > > prox_start;
// 			for (list<pair<int*, pair<float, float> > >::iterator viz_it=viz.begin(); viz_it!=viz.end(); viz_it++){ // coloca os vizinhos em retorno
// 				bool ha = false;
// 				float novoDelta = distance(g, *viz_it, x_target);
// 				if (novoDelta<=delta-1){
// 					prox_start.push_back(make_pair(novoDelta, (*viz_it)));
// 				}
// 				vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
// 				//saber se está na zona de esclusao; se nao estiver, a insere
// 				for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
// 					if (((*viz_it).second.first ==(*it2).second.first && (*viz_it).second.second == (*it2).second.second)){
// 						ha = true;
// 						break;
// 					} else if (((*it2).second.first<=(*viz_it).second.first && (*it2).second.second<=(*viz_it).second.second && ((*it2).second.first<(*viz_it).second.first || (*it2).second.second<(*viz_it).second.second))){
// 						ha = true;
// 						break;
// 					} 
// 					if ((*viz_it).second.first<=(*it2).second.first && (*viz_it).second.second<=(*it2).second.second && ((*viz_it).second.first<(*it2).second.first || (*viz_it).second.second<(*it2).second.second)){
// 						dominadas.push_back(it2); //se o vizinho domina alguem da lista
// 					}
// 				}
// 				if (ha==false){
// 					for (int iii=0; iii<dominadas.size(); iii++){
// 					// 	::delete[] (*dominadas[iii]).first;
// 					 	solucoes.erase(dominadas[iii]); // retira as possiveis solucoes dominadas pela novas
// 					 }
// 					solucoes.push_back((*viz_it));
// 				} //else if ((prox_start.second.first != (*viz_it).second.first || prox_start.second.second != (*viz_it).second.second) && (x_start.second.first != (*viz_it).second.first || x_start.second.second != (*viz_it).second.second)) {
// 					//delete[] (*viz_it).first;
// 				//}
// 				//}
// 			}
// 			//cout<<"\t\t solucoes.size() antes = "<<solucoes.size()<<endl;
// 			if (prox_start.size()>0){
// 				int inx_random = rand()%prox_start.size();
// 				x_start = prox_start[inx_random].second;
// 				delta = prox_start[inx_random].first;
// 				// if (min_delta==delta) delta--;
// 				// else delta = min_delta;
// 				cont++;
// 			} else break;
// 		} else if (cont>=maxSizePath){ //|| solucoes.size()>=maxSolPorTriangulo) {
// 			break;
// 		}
// 	}while(delta>1);
// 	cout<<"\t\tcont = "<<cont<<endl;
// 	//cout<<"\t\tdelta = "<<delta<<endl;
// }



vector<pair <Aresta* , int > > restricted_list(int sizeArestas, float lambda1,float lambda2, Aresta** presort, float num, Conjunto &conjunto){
	Aresta ** L = presort;
	vector<pair <Aresta* , int > > ret;
	bool first = false;
	float w = -1;
	for (int i=0; i<sizeArestas; i++){
		if (L[i] != NULL){
			if (first==false) {
				w = L[i]->getPeso1()*(lambda1) + L[i]->getPeso2()*(lambda2);
				first = true;
			}
			float peso = L[i]->getPeso1()*(lambda1) + L[i]->getPeso2()*(lambda2);
			if (peso<=(1+num)*w){
				if (conjunto.compare(L[i]->getOrigem(), L[i]->getDestino())==false){
					ret.push_back(make_pair(L[i],i));
				} else {
					presort[i] = NULL;
				}
			}
		}
	}
	return ret;
}

pair<int*, pair<float, float> > rmcKruskal(Grafo *g, float lambda1, float lambda2,Aresta** presort, float num){
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
	int cont = 0;
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantArestas(); i++){
		if (g->getStatus(presort[i]->getId())==1){ // se for obrigatoria
			Aresta *a_new = presort[i];
			ret.first[cont++]  = a_new->getId(); 
			ret.second.first  +=a_new->getPeso1();
			ret.second.second +=a_new->getPeso2();
			conjunto.union1(a_new->getOrigem(), a_new->getDestino());
			presort[i] = NULL;
		}
	}
	while (cont<g->getQuantVertices()-1){
		vector<pair <Aresta* , int > > restrictas = restricted_list(g->getQuantArestas(), lambda1,lambda2, presort, num,conjunto);
		//cout<<"restrictas.size() = "<<restrictas.size()<<endl;
		if (restrictas.size()>0){
			int pos = rand()%restrictas.size();
			Aresta *a_new = restrictas[pos].first;
				conjunto.union1(a_new->getOrigem(), a_new->getDestino());
				ret.first[cont++] = a_new->getId(); 
				ret.second.first+=a_new->getPeso1();
				ret.second.second+=a_new->getPeso2();
				presort[restrictas[pos].second] = NULL;
		}
	}

	return ret;
}

//edita sol com uma solucao que minimiza o custo escalarizado
void localSearch(Grafo *g, pair<int*, pair<float, float> >  &sol, float cateto_x, float cateto_y, float escalarX, float escalarY){
	//cout<<"Busca local"<<endl;
	for (int i=0; i<maxSearch; i++){
		list <pair<int*,  pair<float, float> > > viz = vizinhos2(g, sol ,cateto_x, cateto_y, escalarX,escalarY);
		float min = sol.second.first*escalarX + sol.second.second*escalarY;
		list<pair<int*, pair<float, float> > >::iterator it;
		for (list<pair<int*, pair<float, float> > >::iterator it3=viz.begin(); it3!=viz.end(); it3++){
			float custo = (*it3).second.first*escalarX + (*it3).second.second*escalarY;
			if (custo<min){
				min = custo;
				it = it3;
			}
		}
		if (min!=(sol.second.first*escalarX + sol.second.second*escalarY)){
			for (int kgo = 0; kgo<g->getQuantVertices()-1; kgo++){
				sol.first[kgo] = (*it).first[kgo];
			}
			//cout<<"melhorou"<<endl;
			sol.second.first = (*it).second.first;
			sol.second.second = (*it).second.second;
		}
	}

}

void path_relinking(Grafo *g, pair<int*, pair<float, float> > x_startaux, pair<int*, pair<float, float> > x_target, list <pair<int*, pair<float, float> > > &solucoes, float cateto_x, float cateto_y, float escalarX, float escalarY){
	pair<int*, pair<float, float> > x_start = make_pair(new int[g->getQuantVertices()-1], make_pair(x_startaux.second.first, x_startaux.second.second));
	for (int i=0; i<g->getQuantVertices()-1; i++) x_start.first[i] = x_startaux.first[i];
	int delta;
	int cont = 0;
	delta = distance(g, x_start, x_target);
	do {
		cout<<"\t\tdelta = "<<delta<<endl;
		//delta = distance(g, x_start, x_target); 
		if (delta>=1 && cont<maxSizePath){
			list <pair<int*,  pair<float, float> > > viz = vizinhos2(g, x_start,cateto_x, cateto_y, escalarX,escalarY); // somente os vizinhos dentro do triangulo formado pelos catetos x,y
			float min = INT_MAX;
			pair<int*, pair<float, float> > prox_start;
			for (list<pair<int*, pair<float, float> > >::iterator viz_it=viz.begin(); viz_it!=viz.end(); viz_it++){ // coloca os vizinhos em retorno
				//bool ha = false;
				int novoDelta = distance(g, *viz_it, x_target);
				if (novoDelta<=delta-1){
					float custo = (*viz_it).second.first*escalarX + (*viz_it).second.second*escalarY;
					if (custo<min){
						min = custo;
						prox_start = (*viz_it);
					}
				}
			}
			if (min!=INT_MAX){
				x_start = prox_start;
				localSearch(g,x_start, cateto_x, cateto_y, escalarX, escalarY);
				bool ha = false;
				delta = distance(g, x_start, x_target);
				cout<<"\t\tdelta = "<<delta<<endl;
				vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
				//saber se está na zona de esclusao; se nao estiver, a insere
				for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
					if ((x_start.second.first ==(*it2).second.first && x_start.second.second == (*it2).second.second)){
						ha = true;
						break;
					} else if (((*it2).second.first<=x_start.second.first && (*it2).second.second<=x_start.second.second && ((*it2).second.first<x_start.second.first || (*it2).second.second<x_start.second.second))){
						ha = true;
						break;
					} 
					if (x_start.second.first<=(*it2).second.first && x_start.second.second<=(*it2).second.second && (x_start.second.first<(*it2).second.first || x_start.second.second<(*it2).second.second)){
						dominadas.push_back(it2); //se o vizinho domina alguem da lista
					}
				}
				if (ha==false){
					for (int iii=0; iii<dominadas.size(); iii++){
					// 	::delete[] (*dominadas[iii]).first;
					 	solucoes.erase(dominadas[iii]); // retira as possiveis solucoes dominadas pela novas
					 }
					solucoes.push_back(x_start);
				} 
				cont++;
			} else break;
		} else if (cont>=maxSizePath) {
			break;
		}
	}while(delta>1);
	cout<<"\t\tcont = "<<cont<<endl;
}


/* Basseur, Seynhanave e Talbi (2005) sugerem o PLS apos o PR*/
void PLS(Grafo *g, list <pair<int*, pair<float, float> > > &solucoes, float cateto_x, float cateto_y, float escalarX, float escalarY){
	
	for (int i=0; i<maxSearch; i++){
		list <pair<int*,  pair<float, float> > > liii;
		for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
			list <pair<int*,  pair<float, float> > > viz = vizinhos2(g, *it2 ,cateto_x, cateto_y, escalarX,escalarY);
			liii.splice(liii.end(), viz);
		}

		for (list<pair<int*, pair<float, float> > >::iterator it3=liii.begin(); it3!=liii.end(); it3++){
			bool ha=false;
			vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
				
			for (list<pair<int*, pair<float, float> > >::iterator it4=solucoes.begin(); it4!=liii.end(); it4++){ // retira os dominados entre si
				if ((*it4).second.first <= (*it3).second.first && (*it4).second.second <= (*it3).second.second && ((*it4).second.first < (*it3).second.first || (*it4).second.second < (*it3).second.second)){
					ha=true;
					break;
				} else if ((*it4).second.first == (*it3).second.first && (*it4).second.second == (*it3).second.second){
					ha=true;
					break;
				} else if ((*it3).second.first <= (*it4).second.first && (*it3).second.second <= (*it4).second.second && ((*it3).second.first < (*it4).second.first || (*it3).second.second < (*it4).second.second)){
					dominadas.push_back(it4);
				}
			}

			if (ha==false){
				for (int iii=0; iii<dominadas.size(); iii++){
					 solucoes.erase(dominadas[iii]); // retira as possiveis solucoes dominadas pela novas
				}
				solucoes.push_back((*it3));
			} else {
				//deleta
			}
		}

	}
	

}

bool compare(pair<int*, pair<float, float> > p1, pair<int*, pair<float, float> > p2){
 	return (p1.second.first<p2.second.first) || (p1.second.first==p2.second.first && p1.second.second<=p2.second.second);
}



// /* 
// Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
// Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
// Assim, teremos separadas as solucoes suportadas e nao suportadas
// */
list < pair<int*, pair<float, float> > >  phase2KB(Grafo *g, list< pair<int*, pair<float, float> > > &extremas){
	extremas.sort(compare);
	list< pair<int*, pair<float, float> > > noSoportadas; // resultado a ser retornado
	list< pair<int*, pair<float, float> > >::iterator it = extremas.begin(); 
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		list< pair<int*, pair<float, float> > > noSuportadasPQ;
		pair<int*, pair<float, float> > ponto_p = *(it);
		pair<int*, pair<float, float> > ponto_q = *(++it);
		int *p = ponto_p.first; 
		int *q = ponto_q.first;
		
		xp = ponto_p.second.first;
		yp = ponto_p.second.second;
		xq = ponto_q.second.first;
		yq = ponto_q.second.second;
		float minnnn = INT_MAX;
		pair<int*, pair<float, float> > inittt; 
		for (int lklkl = 0; lklkl<5; lklkl++){
			Aresta **arestasPtr = g->getAllArestasPtr();
			mergesort(0, (yp-yq),(xq-xp), 0, arestasPtr, g->getQuantArestas(),3);
			float num = ((float)(rand()%1001))/10000.0;
			pair<int*, pair<float, float> > tree = rmcKruskal(g, (yp-yq),(xq-xp), arestasPtr,num);
			if (tree.second.first<xq && tree.second.second<yp) {
				localSearch(g,tree, xq, yp, (yp-yq), (xq-xp));
				bool ha = false;
				vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
				for (list<pair<int*, pair<float, float> > >::iterator it2=noSuportadasPQ.begin(); it2!=noSuportadasPQ.end(); it2++){
					if ((tree.second.first ==(*it2).second.first && tree.second.second == (*it2).second.second)){
						ha = true;
						break;
					} else if (((*it2).second.first<=tree.second.first && (*it2).second.second<=tree.second.second && ((*it2).second.first<tree.second.first || (*it2).second.second<tree.second.second))){
						ha = true;
						break;
					} 
					if (tree.second.first<=(*it2).second.first && tree.second.second<=(*it2).second.second && (tree.second.first<(*it2).second.first || tree.second.second<(*it2).second.second)){
						dominadas.push_back(it2); //se o vizinho domina alguem da lista
					}
				}
				if (ha==false){
					for (int iii=0; iii<dominadas.size(); iii++){
					// 	::delete[] (*dominadas[iii]).first;
					 	noSoportadas.erase(dominadas[iii]); // retira as possiveis solucoes dominadas pela novas
					 }
					noSuportadasPQ.push_back(tree);
					if ((tree.second.first*(yp-yq) + tree.second.second*(xq-xp))<minnnn){
						minnnn=(tree.second.first*(yp-yq) + tree.second.second*(xq-xp));
						inittt = tree;
					}
				}
			}
		}
		if (minnnn!=INT_MAX){
			cout<<"Triângulo "<<contador<<endl;
			path_relinking(g, inittt, ponto_q, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
			path_relinking(g, ponto_q, inittt, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
			
			path_relinking(g, inittt, ponto_p, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
			path_relinking(g, ponto_p, inittt, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
			// cout<<"Antes PLS = "<<noSuportadasPQ.size()<<endl;
			// PLS(g, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
			// cout<<"Depois PLS = "<<noSuportadasPQ.size()<<endl;
		} //else {
		// 	path_relinking(g, ponto_p, ponto_q, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
		// 	path_relinking(g, ponto_q, ponto_p, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
			
		// }

		
		// 	cout<<"\t\tArea = "<<(yp-yq)*(xq-xp)/2<<endl;
		// 	path_relinking(g, ponto_p, ponto_q, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
		// 	path_relinking(g, ponto_q, ponto_p, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
		// 	//PLS(g, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp));
		// 	cout<<"noSuportadasPQ.size() = "<<noSuportadasPQ.size()<<endl;
			noSoportadas.splice(noSoportadas.end(), noSuportadasPQ);
			
		// cout<<"noSuportadas.size() = "<<noSoportadas.size()<<endl;

		contador++;

	//	cout<<"noSuportadas.size() = "<<noSoportadas.size()<<endl;
	}
	
	return noSoportadas;

}


int main(int argc, const char * argv[]) {
	// verifica numero de parametros
    if (argc != 3) {
        cout << "Parameter error. Usage: " << argv[0] << " (parato front file) (time file) " << endl;
        exit (1);
    }

	srand (time(NULL));
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
	
	 my_grafo.excluiProibidas(); // primeiro, excluimos as proibidas
	 my_grafo.updateIndex(); // depois, atualizamos os idexes das arestas no map
	my_grafo.marcaObrigatorias(); // determinanmos as obrigatorias
	nA= my_grafo.getQuantArestas();

	arestas = my_grafo.get_allArestas();
	arestasPtr = my_grafo.getAllArestasPtr();
 	times(&tempsInit);

	 list <pair<int*, pair<float, float> > > arvores = suportadas(&my_grafo);
	
	cout<<"Fim da primeira fase. Quantidade de solucoes suportadas : "<<arvores.size()<<endl;
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	//cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);


	list <pair<int*, pair<float, float> > > noSuportadas = phase2KB(&my_grafo, arvores);
	

	cout<<"Fim da segunda fase. Quantidade de solucoes nao-suportadas: "<<noSuportadas.size()<<endl;
	cout<<"Quantidade total de solucoes : "<<noSuportadas.size()+arvores.size()<<endl;
	times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	//cout<<user_time2<<endl;
	cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	cout<<"Total: ";
	cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<" segundos"<<endl;

	FILE *paretoFront = fopen(argv[1],"a");
   	FILE *tempofile = fopen(argv[2],"a");
   

   	fprintf(tempofile,"%.10lf\n", (float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK));
   	

	
    int i = 1, cont=0;
 	float max_ob1 = -1, max_ob2 = -1;
    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		
		float cont1=0, cont2=0;
		pair<int*, pair<float, float> > arvr = (*it);
		Conjunto conjjj(n);
   		for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			int iddd = (arvr.first)[a];
				// cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
				if (conjjj.compare(my_grafo.getArestas(iddd)->getOrigem(), my_grafo.getArestas(iddd)->getDestino())==false){
					
					cont1+=my_grafo.getArestas(iddd)->getPeso1();
					cont2+=my_grafo.getArestas(iddd)->getPeso2();
					conjjj.union1(my_grafo.getArestas(iddd)->getOrigem(), my_grafo.getArestas(iddd)->getDestino());
    		
				} else {
					cout<<"ERROOOOOOOOOO"<<endl;
					break;
				}
   		}
   		if (cont1 != arvr.second.first || cont2 != arvr.second.second) {
    		cout<<"ERROROFKROKFORKFORKF 3"<<endl;
    		break;
    	}
    	cout<<arvr.second.first<<" "<<arvr.second.second<<endl;
    	if (arvr.second.first>max_ob1) max_ob1 = arvr.second.first;
    	if (arvr.second.second>max_ob2) max_ob2 = arvr.second.second;
    	fprintf(paretoFront,"%.10lf %.10lf\n", arvr.second.first, arvr.second.second);
   	
	}


	//cout<<"Resultado \n NAO SUPORTADAS"<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		float cont1=0, cont2=0;
		pair<int*, pair<float, float> > arvr = (*it);
		Conjunto conjjj(n);
   		for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			int iddd = (arvr.first)[a];
				// cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
				if (conjjj.compare(my_grafo.getArestas(iddd)->getOrigem(), my_grafo.getArestas(iddd)->getDestino())==false){
					
					cont1+=my_grafo.getArestas(iddd)->getPeso1();
					cont2+=my_grafo.getArestas(iddd)->getPeso2();
					conjjj.union1(my_grafo.getArestas(iddd)->getOrigem(), my_grafo.getArestas(iddd)->getDestino());
    		
				} else {
					cout<<"ERROOOOOOOOOO"<<endl;
					break;
				}
   		}
   		if (cont1 != arvr.second.first || cont2 != arvr.second.second) {
    		cout<<"ERROROFKROKFORKFORKF 3"<<endl;
    		break;
    	}
    	cout<<(*it).second.first<<" "<<(*it).second.second<<endl;
    	if ((*it).second.first>max_ob1) max_ob1 = (*it).second.first;
    	if ((*it).second.second>max_ob2) max_ob2 = (*it).second.second;
    	fprintf(paretoFront,"%.10lf %.10lf\n", (*it).second.first, (*it).second.second);
   	
	}
	
	fprintf(paretoFront,"\n");
   	fclose(paretoFront);
   	fclose(tempofile);

	cout<<"MAXX = "<<max_ob1<<" "<<max_ob2<<endl;
	return 0;
}