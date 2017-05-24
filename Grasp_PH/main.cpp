/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements a hybrid GRASP algorithm with forward relinking (?) Path Relinking
# forward relinking: path-relinking is applied using the worst among xs and xt as the initial solution and the other as the target solution; (Resende e Ribeiro 2005)
#=======================================================================
*/

/*
	Reduz o grafo (SS 2008)
	Calcula vetores de escalarizacao (SS 2008) Armezenar numa arvore binaria balaciada
	Grasp{ (Baseado em parte no trabalho de Arroyo e Vianna)
		Constroi solucao 
		Busca local (PLS, baseado no algoritmo proposto por Drugan e Thierens (2012))
		PH truncado (Resende e Ribeiro (2005) e Laguna and Marti (1999))
	}
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

#define maxIteracoes 3
#define maxSizePath 8 // 10 é um bom valor (a priori)


/*	TODO: alterar a funcao vizinho do modo a retornar somente visinho diferentes, como foi feito no SRPR
*/

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;
float yp, yq, xp, xq;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinalBuscaLocal ; // para medir o tempo
vector< pair<float, Aresta** > > preSorting;

bool isEgalObjetive(float t1_peso1, float t1_peso2, float t2_peso1, float t2_peso2){
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);
}



void borderSearch(Grafo *g, list<pair<int*, pair<float, float> > > &resul, pair<int*, pair<float, float> > &sl, pair<int*, pair<float, float> > &sll){ 
	int *s1 = sl.first;
	int *s2 = sll.first;
	int * A2;
	stack<pair<int*,int*> >  pilha;
	stack<pair<float, float> > pilhaX;
	stack<pair<float, float> > pilhaY;
	stack<pair<int,list<pair<int*, pair<float, float> > >::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	

	pilha.push(make_pair(s1, s2));
	pilhaX.push(make_pair(sl.second.first, sll.second.first)); //x's da primeira e sefunda solucao
	pilhaY.push(make_pair(sl.second.second, sll.second.second)); //y's da primeira e segunda solucao
	pilhaIt.push(make_pair(2, resul.begin()));


	while (pilha.size()!=0){
		pair<int*,int*> sols = pilha.top();
		pilha.pop();
		s1 = sols.first;
		s2 = sols.second;
		pair<int,list<pair<int*, pair<float, float> > >::iterator>  it = pilhaIt.top();
		pilhaIt.pop();

		pair<float, float> xs = pilhaX.top();
		pair<float, float> ys =  pilhaY.top();
		float xl = xs.first;
		float yl = ys.first;
		float xll = xs.second;
		float yll = ys.second;

		pilhaX.pop();
		pilhaY.pop();

		//A2 = new int[g->getQuantArestas()];
		A2 = new int[g->getQuantVertices()-1];
		float cont; // nao utilisazado nesse caso
		mergesort(xl, yl, xll, yll, arestasPtr, g->getQuantArestas(),3);
		float x, y;
		kruskal(g, arestasPtr,A2,x, y);
		if( !( (isEgalObjetive(x, y, xl, yl)) || (isEgalObjetive(x, y, xll, yll)) ) ){
			if (it.first == 1){ // antes
				resul.insert(it.second, make_pair(A2, make_pair(x,y)));//A2); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, make_pair(A2, make_pair(x,y)));
				it.second--;// it agora aponta para o item  A2
			}

			pilha.push(make_pair(A2, s2)); // L''
			pilhaIt.push(make_pair(2,it.second)); 
			pilhaX.push(make_pair(x, xll));
			pilhaY.push(make_pair(y, yll));

			pilha.push(make_pair(s1, A2));  // L'
			pilhaIt.push(make_pair(1,it.second)); 
			pilhaX.push(make_pair(xl, x));
			pilhaY.push(make_pair(yl, y));
			
		}
	
	}
}


list <pair<int*, pair<float, float> > > buscaDicotomica(Grafo *g){
	list< pair<int*, pair<float, float> > > result;
	int *s1 = new int[g->getQuantVertices()-1];
	float xr, yr; // nao utilisazado nesse caso
	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),1);
	kruskal(g, arestasPtr,s1,xr, yr); // arvore para o primeiro objetivo
	 pair<int*, pair<float, float> > ps1 = make_pair(s1, make_pair(xr,yr));
	result.push_back(ps1);
	int* s2 = new int[g->getQuantVertices()-1];
	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),2);
	float xr2, yr2;
	kruskal(g, arestasPtr, s2,xr2, yr2); // arvore para o segundo objetivo
	pair<int*, pair<float, float> > ps2 = make_pair(s2, make_pair(xr2,yr2));
	
	if (isEgalObjetive(xr, yr, xr2, yr2)==false){
		borderSearch(g, result, ps1, ps2);
		result.push_back(ps2);
	}
	return result;
 }

/* 	
	uma arvore s' é vizinha de s sse elas diferem em somente uma aresta
	retorna os vizinhos de "s" que nao sao dominados por "s"
*/
list <pair<int*,  pair<float, float> > > vizinhos(Grafo *g, pair<int*, pair<float, float> > sol){
	list <pair<int*,  pair<float, float> > > retorno;
	for (int a = 0; a<g->getQuantVertices()-1; a++){ // n-1 (aresta que sairá)
		if (g->getStatus(sol.first[a])==0){ // so sai se for opcional. Se for obrigatoria, nao sai
			int idArestaSai = sol.first[a];
			Conjunto conjunto(g->getQuantVertices());
			for (int i = 0; i <g->getQuantVertices()-1; i++){
				if (i!=a){
					conjunto.union1(g->getArestas(sol.first[i])->getOrigem(), g->getArestas(sol.first[i])->getDestino());	
				}
			}
			int peso1base = sol.second.first - g->getArestas(sol.first[a])->getPeso1();
			int peso2base = sol.second.second - g->getArestas(sol.first[a])->getPeso2();
			vector<int> arestasPossiveis;
		 	for (int i = 0; i <g->getQuantArestas(); i++){
		 		if (i!=idArestaSai){
		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
		 				int newPeso1 = peso1base+g->getArestas(i)->getPeso1();
		 				int newPeso2 = peso2base+g->getArestas(i)->getPeso2();
		 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
		 					int *base = new int[g->getQuantVertices()-1];
		 					for (int are = 0; are <g->getQuantVertices()-1; are++){
		 						if (are == a) base[are] = i;
		 						else base[are] = sol.first[are];
		 					} 
		 					retorno.push_back(make_pair(base, make_pair(newPeso1,newPeso2)));	
		 					
		 				}
		 			}
		 		} 
		 	}
		 }
	}
	return retorno;
}


// /* 	
// 	uma arvore s' é vizinha de s sse elas diferem em somente uma aresta
// 	retorna os vizinhos de "s" que nao sao dominados por "s"
// 	esta funcao nao retorna todos os vizinhos, mas um subconjunto deles
// */
// list <pair<int*,  pair<float, float> > > vizinhos2(Grafo *g, pair<int*, pair<float, float> > sol, int maxVizinhos){
// 	list <pair<int*,  pair<float, float> > > retorno;
// 	vector <int > opcionais; // id das arestas opcionais que estao em sol
// 	map<int, vector<int> > memoria; // para cada aresta que sai (int) as possiveis arestas que entram (vector)
// 	for (int a = 0; a<g->getQuantVertices()-1; a++){ // n-1 (aresta que sairá)
// 		if (g->getStatus(sol.first[a])==0){ 
// 			opcionais.push_back(sol.first[a]);
// 			vector<int> entra;
// 			memoria[sol.first[a]] = entra;
// 		}
// 	}
// 	do{
// 		int a = rand()%(opcionais.size()); // aresta que sairá
// 		int idArestaSai = opcionais[a]; //sol.first[a];
// 		if (memoria[idArestaSai].size() == 0){
// 			Conjunto conjunto(g->getQuantVertices());
// 			for (int i = 0; i <g->getQuantVertices()-1; i++){
// 				if (sol.first[i]!=opcionais[a]){
// 					conjunto.union1(g->getArestas(sol.first[i])->getOrigem(), g->getArestas(sol.first[i])->getDestino());	
// 				}
// 			}
// 			int peso1base = sol.second.first - g->getArestas(idArestaSai)->getPeso1();
// 			int peso2base = sol.second.second - g->getArestas(idArestaSai)->getPeso2();
// 			for (int i = 0; i <g->getQuantArestas(); i++){
// 		 		if (i!=idArestaSai){
// 		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
// 		 				int newPeso1 = peso1base+g->getArestas(i)->getPeso1();
// 		 				int newPeso2 = peso2base+g->getArestas(i)->getPeso2();
// 		 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
// 		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
// 		 					memoria[idArestaSai].push_back(i);
// 		 				}
// 		 			}
// 		 		} 
// 		 	}
// 		}
// 		if (memoria[idArestaSai].size()>0){
// 			int indexEntra = rand()%(memoria[idArestaSai].size());
// 			int idEntra = memoria[idArestaSai][indexEntra];
// 			int newPeso1 = sol.second.first  - g->getArestas(idArestaSai)->getPeso1()+g->getArestas(idEntra)->getPeso1();
// 		 	int newPeso2 = sol.second.second - g->getArestas(idArestaSai)->getPeso2()+g->getArestas(idEntra)->getPeso2();
// 			int *base = new int[g->getQuantVertices()-1];
// 			for (int are = 0; are <g->getQuantVertices()-1; are++){
// 				if (sol.first[are] != idArestaSai) base[are] = sol.first[are];
// 				else base[are] = idEntra;
// 			} 
// 			retorno.push_back(make_pair(base, make_pair(newPeso1,newPeso2)));
// 			memoria[idArestaSai].erase(memoria[idArestaSai].begin()+indexEntra);
// 			if (memoria[idArestaSai].size()==0) {
// 				//memoria.erase(idArestaSai);
// 				opcionais.erase(opcionais.begin()+a);
// 			}
// 		} else {
// 			opcionais.erase(opcionais.begin()+a);
// 		}
// 	} while (retorno.size()<maxVizinhos && opcionais.size()>0);
// 	return retorno;
// }

int distance(Grafo *g, pair<int*, pair<float, float> > xi, pair<int*, pair<float, float> > xt){
	int cont = 0;
	bool tem = false;
	if (xi.second.first == xt.second.first && xi.second.second == xt.second.second) return 0;
	for (int i=0; i<g->getQuantVertices()-1; i++){
		tem = false;
		for (int t=0; t<g->getQuantVertices()-1; t++){
			if (xt.first[t] == xi.first[i]) {
				tem = true; break;
			}
		}
		if (tem==false) cont++;
	}
	return cont;
}

void path_relinking(Grafo *g, pair<int*, pair<float, float> > x_startaux, pair<int*, pair<float, float> > x_target, list <pair<int*, pair<float, float> > > &solucoes, float lambda){
	pair<int*, pair<float, float> > x_start = make_pair(new int[g->getQuantVertices()-1], make_pair(x_startaux.second.first, x_startaux.second.second));
	for (int i=0; i<g->getQuantVertices()-1; i++) x_start.first[i] = x_startaux.first[i];
	pair<int*, pair<float, float> > prox_start = make_pair(new int[g->getQuantVertices()-1], make_pair(0, 0));;
	int delta;
	int cont = 0;
	delta = distance(g, x_start, x_target);
	do {
		if (delta>1 && cont<maxSizePath){
			//cout<<"ANTES"<<endl;
			list <pair<int*,  pair<float, float> > > viz = vizinhos(g, x_start);
			//cout<<"DEPOIS"<<endl;
			float min = INT_MAX;
			int min_delta = delta;
			
			for (list<pair<int*, pair<float, float> > >::iterator viz_it=viz.begin(); viz_it!=viz.end(); viz_it++){ // coloca os vizinhos em retorno
				bool ha = false;
				for (list<pair<int*, pair<float, float> > >::iterator viz_it2=viz_it; viz_it2!=viz.end(); viz_it2++){
					if (((*viz_it2).second.first<=(*viz_it).second.first && (*viz_it2).second.second<=(*viz_it).second.second && ((*viz_it2).second.first<(*viz_it).second.first || (*viz_it2).second.second<(*viz_it).second.second))){
						// se viz_it2 dominar viz_it,, remove viz_it
						delete[] (*viz_it).first;
						// viz.erase(viz_it);
						// if (viz_it != viz.begin()) viz_it--;
						ha = true;
						break;
					} 
				}
				if (ha == false){
					int novoDelta = distance(g, *viz_it, x_target);
					if (novoDelta<=delta-1){
						float custo = (*viz_it).second.first*lambda + (*viz_it).second.second*(1-lambda);
						if (custo<min){
							min = custo;
							//prox_start = (*viz_it);
							for (int i=0; i<g->getQuantVertices()-1; i++) prox_start.first[i] = (*viz_it).first[i];
							prox_start.second.first = (*viz_it).second.first;
							prox_start.second.second = (*viz_it).second.second;
							min_delta= novoDelta;
						}
					}
					//bool ha = false;
					vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
					for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
						if (((*viz_it).second.first ==(*it2).second.first && (*viz_it).second.second == (*it2).second.second)){
							ha = true;
							break;
						} else if (((*it2).second.first<=(*viz_it).second.first && (*it2).second.second<=(*viz_it).second.second && ((*it2).second.first<(*viz_it).second.first || (*it2).second.second<(*viz_it).second.second))){
							ha = true;
							break;
						} 
						if ((*viz_it).second.first<=(*it2).second.first && (*viz_it).second.second<=(*it2).second.second && ((*viz_it).second.first<(*it2).second.first || (*viz_it).second.second<(*it2).second.second)){
							dominadas.push_back(it2); //se o vizinho domina alguem da lista
						}

					}
					if (ha==false){
						for (int iii=0; iii<dominadas.size(); iii++){
							//delete[] (*dominadas[iii]).first;
						 	solucoes.erase(dominadas[iii]);
						 }
						solucoes.push_back((*viz_it));
					} else if ((prox_start.second.first != (*viz_it).second.first || prox_start.second.second != (*viz_it).second.second) && (x_start.second.first != (*viz_it).second.first || x_start.second.second != (*viz_it).second.second)) {
						delete[] (*viz_it).first;
					}
				}
			}
			if (min!=INT_MAX){
				for (int i=0; i<g->getQuantVertices()-1; i++) x_start.first[i] = prox_start.first[i];
				x_start.second.first = prox_start.second.first;
				x_start.second.second = prox_start.second.second;
				if (min_delta==delta) delta--;
				else delta = min_delta;
				cont++;
			} else break;
		} else if (cont>=maxSizePath) break;
	}while(delta>1);
	delete[] x_start.first;
	delete[] prox_start.first;
}

void localSearch(Grafo *g, pair<int*, pair<float, float> > sol1,list <pair<int*, pair<float, float> > > &solucoes, float lambda,vector <pair<int*, pair<float, float> > >&elite){
	
	int contIteracoes = 0;
	bool ha ;
	vector <pair<int*, pair<float, float> > > novasSolucoes;
	novasSolucoes.push_back(sol1);
	while (contIteracoes<novasSolucoes.size() && contIteracoes<maxIteracoes){
		
		pair<int*, pair<float, float> > sol = novasSolucoes[contIteracoes];
		//cout<<"ANTESS"<<endl;
		list <pair<int*,  pair<float, float> > > viz = vizinhos(g, sol);
		//cout<<"DEPOIS"<<endl;
		// é garantido que os vizinhos de "itlocal" nao sao dominados por "itlocal"
		for (list<pair<int*, pair<float, float> > >::iterator viz_it=viz.begin(); viz_it!=viz.end(); viz_it++){ // coloca os vizinhos em retorno

			ha = false;
			// ESTE FOR ABAIXO É IMPORTANTISSIMO 
			/* ele se encarrega de selecionar os vizinhos promissores*/
			for (list<pair<int*, pair<float, float> > >::iterator viz_it2=viz_it; viz_it2!=viz.end(); viz_it2++){
				if (((*viz_it2).second.first<=(*viz_it).second.first && (*viz_it2).second.second<=(*viz_it).second.second && ((*viz_it2).second.first<(*viz_it).second.first || (*viz_it2).second.second<(*viz_it).second.second))){
					// se viz_it2 dominar viz_it,, remove viz_it
					delete[] (*viz_it).first;
					ha = true;
					break;
				} 
			}
			if (ha == false){
				vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
				for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
					if (((*viz_it).second.first ==(*it2).second.first && (*viz_it).second.second == (*it2).second.second)){
						ha = true;
						break;
					} else if (((*it2).second.first<=(*viz_it).second.first && (*it2).second.second<=(*viz_it).second.second && ((*it2).second.first<(*viz_it).second.first || (*it2).second.second<(*viz_it).second.second))){
						ha = true;
						break;
					} 
					if ((*viz_it).second.first<=(*it2).second.first && (*viz_it).second.second<=(*it2).second.second && ((*viz_it).second.first<(*it2).second.first || (*viz_it).second.second<(*it2).second.second)){
						dominadas.push_back(it2);
					}

				}
				if (ha==false){
					for (int iii=0; iii<dominadas.size(); iii++){
						//delete[] (*dominadas[iii]).first;
						solucoes.erase(dominadas[iii]);
						// for (int jjj=0; jjj<novasSolucoes.size(); jjj++){
						// 	if ((*dominadas[iii]).second.first == novasSolucoes[jjj].second.first && (*dominadas[iii]).second.second == novasSolucoes[jjj].second.second){
						// 		novasSolucoes.erase(novasSolucoes.begin()+jjj);
						// 		if (jjj<novasSolucoes.size()-1) jjj--;
						// 	}
						// }
					} 
					 solucoes.push_back((*viz_it));
					
					novasSolucoes.push_back((*viz_it));
				} else {
					delete[] (*viz_it).first;
				}
			}
		}
		
		contIteracoes++;
	}
	cout<<"novasSolucoes.size() = "<<novasSolucoes.size()<<endl;
	if (novasSolucoes.size()>1){
		int sorteia, indexElite;
		sorteia = rand()%(novasSolucoes.size()-1)+1;
		indexElite = rand()%elite.size();
		// cout<<"indexElite = "<<indexElite<<endl;
		// cout<<"sorteia = "<<sorteia<<endl;
		//path_relinking(g, elite[indexElite], novasSolucoes[sorteia], solucoes,lambda);
		path_relinking(g,  novasSolucoes[sorteia], elite[indexElite], solucoes,lambda);
		
		cout<<"Size solucoes = "<<solucoes.size()<<endl;
	}
}

bool compare(pair<int*, pair<float, float> > p1, pair<int*, pair<float, float> > p2){
 	float escl1 = p1.second.first*(yp-yq)+p1.second.second*(xq-xp);
 	float escl2 = p2.second.first*(yp-yq)+p2.second.second*(xq-xp);
	return (escl1<escl2);
}


void grasp(Grafo *g, float alfa, list <pair<int*, pair<float, float> > > &solucoes){
	list <pair<int*, pair<float, float> > > elite1 = buscaDicotomica(g);
	vector <pair<int*, pair<float, float> > > elite;
	for (list<pair<int*, pair<float, float> > >::iterator it222=elite1.begin(); it222!=elite1.end(); it222++){
		elite.push_back(*it222);
	}
	cout<<"preSorting.size() = "<<preSorting.size()<<endl;
	for (int itera = 0; itera<preSorting.size(); itera++){
		int *nova = new int[g->getQuantVertices()-1];
		pair<float, Aresta**> presort = preSorting[itera];
		float peso1=0, peso2=0;
		int cont = 0;
		Conjunto conc(g->getQuantVertices());
		vector<Aresta *> amostral;
		for (int i=0; i<g->getQuantArestas(); i++){
			Aresta *a = presort.second[i];
			if (g->getStatus(a->getId())==1){ // obrigatoria
				nova[cont++] = a->getId();
				conc.union1(a->getOrigem(), a->getDestino());
				peso1+= a->getPeso1();
				peso2+= a->getPeso2();
			} else{
				amostral.push_back(a);
			}
		}
		int rlc = alfa*amostral.size();
		if (rlc<1) rlc = 1;
		while (cont<g->getQuantVertices()-1 && rlc>0){
			int index = rand()%(rlc); // entre 0 e rlc-1
			Aresta *a = amostral[index];
			if (conc.compare(a->getOrigem(),a->getDestino())==false){
				nova[cont++] = a->getId();
				conc.union1(a->getOrigem(),a->getDestino());
				peso1+= a->getPeso1();
				peso2+= a->getPeso2();
			
			} 
			amostral.erase(amostral.begin()+index);
			rlc--;
		}
		if (cont<g->getQuantVertices()-1){ // se por acaso, o laço acima terminar sem uma arvore estar formada, entao adcionamos as menores arestas
			for (int i=0; i<amostral.size() && cont<g->getQuantVertices()-1; i++){
				Aresta *a = amostral[i];
				if (conc.compare(a->getOrigem(),a->getDestino())==false){
					nova[cont++] = a->getId();
					conc.union1(a->getOrigem(),a->getDestino());
					peso1+= a->getPeso1();
					peso2+= a->getPeso2();
				} 
			}
		}
		if (cont==g->getQuantVertices()-1){
			pair<int*, pair<float, float> > sol1 = make_pair(nova, make_pair(peso1,peso2));
			bool ha = false;
			vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
			for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
				if ((sol1.second.first ==(*it2).second.first && sol1.second.second == (*it2).second.second)){
					ha = true;
					break;
				} else if (((*it2).second.first<=sol1.second.first && (*it2).second.second<=sol1.second.second && ((*it2).second.first<sol1.second.first || (*it2).second.second<sol1.second.second))){
					ha = true;
					break;
				} 
				if (sol1.second.first<=(*it2).second.first && sol1.second.second<=(*it2).second.second && (sol1.second.first<(*it2).second.first || sol1.second.second<(*it2).second.second)){
					dominadas.push_back(it2);
				}
			}
			if (ha == false) {
				for (int iii=0; iii<dominadas.size(); iii++){
					delete[] (*dominadas[iii]).first;
					solucoes.erase(dominadas[iii]);
				}
				solucoes.push_back(sol1);
				localSearch(g, sol1, solucoes,presort.first,elite);
				
			} else {
				delete[] sol1.first;
			}
		}
		delete[] preSorting[itera].second;
	}
}

int main(){
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

	times(&tempsInit); // o tempo inclui o calculo dos vetores

 	my_grafo.computaPreSorting();
	preSorting = my_grafo.getPreSorting();

 	list <pair<int*, pair<float, float> > > arvores;//=buscaDicotomica(&my_grafo);
	grasp(&my_grafo, 0.01, arvores);
	//cout<<"Fim da primeira fase ... "<<endl;
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	//cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<" segundos"<<endl;//"Tempo do usuario por segundo : "
   	
	
    int i = 1, cont=0;
 
 	float peso1max = -1, peso2max = -1;
 	float cont1=0, cont2=0;
    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		pair<int*, pair<float, float> > arvr;
		arvr = (*it);
		//cout<<"Arvore "<<i<<endl;
		cont1=0; cont2=0;
    	for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			int iddd = (arvr.first)[a];
				// cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
    // 			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
    			cont1+=my_grafo.getArestas(iddd)->getPeso1();
				cont2+=my_grafo.getArestas(iddd)->getPeso2();
    		
    	}
    	if (cont1!=arvr.second.first || cont2!=arvr.second.second) cout<<"ERRRRRRROOOOOO   :    ";
    	cout<<arvr.second.first<<" "<<arvr.second.second<<endl;
    	if (arvr.second.first>peso1max) peso1max = arvr.second.first;
    	if (arvr.second.second>peso2max) peso2max = arvr.second.second ;
    	i++;

	}
	cout<<"MAX = "<<peso1max<<" "<<peso2max<<endl;
	
	return 0;
}