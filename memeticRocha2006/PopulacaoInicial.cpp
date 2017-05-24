#include "PopulacaoInicial.h"
#include "Conjunto.h"
#include <stdlib.h>     /* srand, rand */
#include "Parametros.h"
#include "margeSort.h"
#include <iostream>

pair<int*, pair<float, float> > clone(Grafo *g, pair<int*, pair<float, float> > &s){
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
	for (int i=0;i<g->getQuantVertices()-1; i++) ret.first[i] = s.first[i];
	ret.second.first = s.second.first;
	ret.second.second = s.second.second;
	return ret;
}


vector<pair <Aresta* , int > > restricted_list(int sizeArestas, float lambda, Aresta** presort, float num, Conjunto &conjunto){
	Aresta ** L = presort;
	vector<pair <Aresta* , int > > ret;
	bool first = false;
	float w = -1;
	for (int i=0; i<sizeArestas; i++){
		if (L[i] != NULL){
			if (first==false) {
				w = L[i]->getPeso1()*(lambda) + L[i]->getPeso2()*(1-lambda);
				first = true;
			}
			float peso = L[i]->getPeso1()*(lambda) + L[i]->getPeso2()*(1-lambda);
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

pair<int*, pair<float, float> > rmcKruskal(Grafo *g, float lambda,Aresta** presort, float num){
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
		vector<pair <Aresta* , int > > restrictas = restricted_list(g->getQuantArestas(), lambda, presort, num,conjunto);
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

// vector<pair <Aresta* , int > > restricted_list(int sizeArestas, float lambda, Aresta** presort, float num){
// 	Aresta ** L = presort;
// 	vector<pair <Aresta* , int > > ret;
// 	bool first = false;
// 	float w = -1;
// 	for (int i=0; i<sizeArestas; i++){
// 		if (L[i] != NULL){
// 			if (first==false) {
// 				w = L[i]->getPeso1()*(lambda) + L[i]->getPeso2()*(1-lambda);
// 				first = true;
// 			}
// 			float peso = L[i]->getPeso1()*(lambda) + L[i]->getPeso2()*(1-lambda);
// 			if (peso<=(1+num)*w){
// 				ret.push_back(make_pair(L[i],i));
// 			}
// 		}
// 	}
// 	return ret;
// }
// pair<int*, pair<float, float> > rmcKruskal(Grafo *g, float lambda,Aresta** presort, float num){
// 	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
// 	int cont = 0;
// 	Conjunto conjunto(g->getQuantVertices());
// 	while (cont<g->getQuantVertices()-1){
// 		vector<pair <Aresta* , int > > restrictas = restricted_list(g->getQuantArestas(), lambda, presort, num);
// 		cout<<" \t \t restrictas.size() = "<<restrictas.size()<<endl;
// 		cout<< "\t \t cont = "<<cont<<endl;
// 		if (restrictas.size()>0){
// 			int pos = rand()%restrictas.size();
// 			Aresta *a_new = restrictas[pos].first;
// 			if (conjunto.compare(a_new->getOrigem(), a_new->getDestino())==false){
// 				conjunto.union1(a_new->getOrigem(), a_new->getDestino());
// 				ret.first[cont++] = a_new->getId(); 
// 				ret.second.first+=a_new->getPeso1();
// 				ret.second.second+=a_new->getPeso2();
// 			}
// 			presort[restrictas[pos].second] = NULL;
// 		}
// 	}

// 	return ret;
// }

// este procedimento é baseado no procedimento RandWalkRST
pair<int*, pair<float, float> > RandomWalk(Grafo *g){
	int v = rand()%g->getQuantVertices(); // vertice inicial
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
	int cont = 0;
	bool isVisitado[g->getQuantVertices()];
	for (int i=0; i<g->getQuantVertices(); i++)isVisitado[i] = false;
	isVisitado[v] = true;
	while (cont<g->getQuantVertices()-1){
		vector<int> arestasAdjacentes;
		//olha os vizinhos de v
		for (int i=0; i<g->getVertice(v)->getGrau(); i++){
			Aresta *a = g->getVertice(v)->getAresta(i);
			int vprox = a->getDestino();
			if (vprox == v) vprox = a->getOrigem();
			//agora verificamos se o vertice vizinho nao foi vizitado
			if (isVisitado[vprox] == false){
				arestasAdjacentes.push_back(a->getId());
			}
		}

		if (arestasAdjacentes.size()>0){
			int are = rand()%(arestasAdjacentes.size());
			Aresta * a = g->getArestas(arestasAdjacentes[are]);
			ret.first[cont++] = a->getId();
			ret.second.first+=a->getPeso1();
			ret.second.second+=a->getPeso2();
			int vprox = a->getDestino();
			if (vprox == v) vprox = a->getOrigem();
			isVisitado[vprox] = true;
			v = vprox;
		}
	}
	return ret;
} 

// retorna true se a tree ja existe no vetor
bool thereis(vector< pair<int*, pair<float, float> > > &vetor, pair<int*, pair<float, float> > &tree ){
	for (int i=0; i<vetor.size(); i++){
		if (vetor[i].second.first == tree.second.first && vetor[i].second.second == tree.second.second) return true;
	}
	return false;
}

vector< pair<int*, pair<float, float> > > getPopulacaoInicial(Grafo *g, Arquivo &global_arc){
	float passo = 1.0/max_rmckrus;
	vector< pair<int*, pair<float, float> > > ret;
	//int conttttt=0;
	for (float i=passo; i<1.0; i+=passo){
		Aresta **arestasPtr = g->getAllArestasPtr();
		float lambda = i;
		mergesort(0, lambda, 1.0-lambda, 0, arestasPtr, g->getQuantArestas(),3);
		float num = ((float)(rand()%1001))/10000.0;
		pair<int*, pair<float, float> > tree = rmcKruskal(g, lambda, arestasPtr,num);
		cout<<lambda<<endl;
		//if (thereis(ret, tree) == false) {
			ret.push_back(tree);
			global_arc.adicionarSol(clone(g, tree));
			//conttttt = 0;
		//} 
		// else {
		// 	conttttt++;
		// 	if (conttttt <= 10) i-=passo;
		// }
	}
	cout<<"Populacao do rmckrus ok"<<endl;
	//conttttt=0;
	for (int i=0; i<(size_pop-max_rmckrus);i++){
		pair<int*, pair<float, float> > tree = RandomWalk(g);
		//if (thereis(ret, tree) == false) {
			ret.push_back(tree);
			global_arc.adicionarSol(clone(g, tree));
			//conttttt = 0;
		//} 
		// else {
		// 	conttttt++;
		// 	if (conttttt <= 10) i--;
		// }
	}
	cout<<"Populacao do RandomWalk ok"<<endl;
	return ret;
}

