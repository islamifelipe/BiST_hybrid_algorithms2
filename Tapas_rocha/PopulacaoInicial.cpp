
#include "PopulacaoInicial.h"
#include "Conjunto.h"
#include <stdlib.h>     /* srand, rand */
#include "Parametros.h"
#include "margeSort.h"
#include "kruskal.h"
#include <iostream>
#include <queue>          // std::queue
#include <cmath>

typedef struct { // pra calcular as solucoes suportadas
	pair<int*, pair<float, float> > s1; 
	pair<int*, pair<float, float> > s2; 
} ItemFila;
/*Esta funcao é adptada do algoritmo propsoto na dissertacao de Monteiro (2010)
O procedimento usa uma filha pra obter solucoes suportadas bem distribuidas na fronteira de pareto
O procedimento termina ou quando o limite de iteracoes é atingido ou quando acaba as solucoes suportadas*/
void suportadas(Grafo *g, vector < pair<int*, pair<float, float> > > &setSolucoes, Arquivo &global_arc){
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
	global_arc.adicionarSol(clone(g, ps1));
	global_arc.adicionarSol(clone(g, ps2));

	int limite  = (size_pop*por_max_suportadas)/100;
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
				
			if (cont<limite){
				setSolucoes.push_back(novo);
				cont++;
			}
			global_arc.adicionarSol(clone(g, novo));
			fila.push((ItemFila){s1, novo});
			fila.push((ItemFila){novo, s2});
		}

	}

}


pair<int*, pair<float, float> > clone(Grafo *g, pair<int*, pair<float, float> > &s){
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
	for (int i=0;i<g->getQuantVertices()-1; i++) ret.first[i] = s.first[i];
	ret.second.first = s.second.first;
	ret.second.second = s.second.second;
	return ret;
}


vector<pair <Aresta* , int > > restricted_list(Grafo *g, float lambda1,float lambda2, Aresta** L, float num, Conjunto &conjunto){
	//Aresta ** L = presort;
	vector<pair <Aresta* , int > > ret;
	bool first = false;
	float w = -1;
	for (int i=0; i<g->getQuantArestas(); i++){
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
					L[i] = NULL;
				}
			}
		}
	}
	return ret;
}

pair<int*, pair<float, float> > rmcKruskal(Grafo *g,float lambda1, float lambda2,Aresta** presort, float num,  int arestasObrigatorias){
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
	int cont = 0;
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantArestas() && arestasObrigatorias>0; i++){
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
		vector<pair <Aresta* , int > > restrictas = restricted_list(g, lambda1,lambda2, presort, num,conjunto);
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

// este procedimento é baseado no procedimento RandWalkRST
pair<int*, pair<float, float> > RandomWalk(Grafo *g, int quantObrigatorias){
	int v = rand()%g->getQuantVertices(); // vertice inicial
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
	int cont = 0;
	// bool isVisitado[g->getQuantVertices()];
	// for (int i=0; i<g->getQuantVertices(); i++)isVisitado[i] = false;
	// isVisitado[v] = true;
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantArestas() && quantObrigatorias>0; i++){
		if (g->getStatus(i)==1){ // se for obrigatoria
			ret.first[cont++]  = i; 
			ret.second.first  += g->getArestas(i)->getPeso1();
			ret.second.second += g->getArestas(i)->getPeso2();
			conjunto.union1( g->getArestas(i)->getOrigem(),  g->getArestas(i)->getDestino());
		}
	}

	vector<int> amostral;
	for (int i=0; i<g->getQuantArestas(); i++){
		if (g->getStatus(i)!=1){
			amostral.push_back(i);
		}
	}
	while (cont<g->getQuantVertices()-1 && amostral.size()>0){
		int are = rand()%(amostral.size());
		Aresta * a = g->getArestas(amostral[are]);

		if (conjunto.compare(a->getOrigem(), a->getDestino())==false){
			ret.first[cont++] = a->getId();
			ret.second.first+=a->getPeso1();
			ret.second.second+=a->getPeso2();
			conjunto.union1(a->getOrigem(), a->getDestino());
		}
		amostral.erase(amostral.begin()+are);
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

vector< pair<int*, pair<float, float> > > getPopulacaoInicial(Grafo *g, Arquivo &global_arc, int quantObrigatorias){
	vector< pair<int*, pair<float, float> > > ret;
	suportadas(g, ret, global_arc);
	cout<<ret.size()<<" solucoes suportadas"<<endl;
	int quantKruskal = (size_pop*por_max_rmckrus)/100;
	//float passo = 1.0/quantKruskal;

	for (float i=1; i<quantKruskal; i++){
		float lambda1 = i/quantKruskal;
		float lambda2 = 1.0 - lambda1;
		// cout<<"lambda1 = "<<lambda1<<endl;
		// cout<<"lambda2 = "<<lambda2<<endl;
		Aresta **arestasPtr = g->getAllArestasPtr();
		mergesort(0, lambda1, lambda2, 0, arestasPtr, g->getQuantArestas(),3);
		float num = ((float)(rand()%1001))/10000.0;
		pair<int*, pair<float, float> > tree = rmcKruskal(g, lambda1, lambda2, arestasPtr,num,quantObrigatorias);
		//if (thereis(ret, tree) == false) {
			ret.push_back(tree);
			global_arc.adicionarSol(clone(g, tree));
		//} //else i-=passo;
	}
	cout<<ret.size()<<" individuos OBTIDOS APOS rmcKruskal"<<endl;
	int sizee =  ret.size();
	for (int i=0; i<(size_pop - sizee);i++){
		pair<int*, pair<float, float> > tree = RandomWalk(g, quantObrigatorias);
		//if (thereis(ret, tree) == false) {
			ret.push_back(tree);
			global_arc.adicionarSol(clone(g, tree));
		//}// else i--;
	}
	cout<<ret.size()<<" individuos na populacao inicial"<<endl;
	cout<<global_arc.getSize()<<" solucoes no arquivo inicial"<<endl;
	
	return ret;
}

