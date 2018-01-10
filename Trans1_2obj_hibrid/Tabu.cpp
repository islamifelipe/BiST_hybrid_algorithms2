#ifndef TABU_CPP
#define TABU_CPP

typedef struct {
	int origem;
	int destino; 
	int tempo; /*inicialmente é tabutenure. Ai vai diminuindo a cada iteraçao. Quando chegar em zero, sai*/ 
} tabuItem;

#include <cassert>
#include "SolucaoEdgeSet.cpp"
#include "BoundedParetoSet.cpp"
#include "param.h"
#include <list>
#include <string>
#include <iostream>
using namespace std;

extern BoundedParetoSet arc_global;

//retorna “verdadeiro” caso sl esteja em uma regiao menos populosa que s com relacao ao arquivo local_arc.
bool m_grid( BoundedParetoSet &arquivo, SolucaoEdgeSet *sl, SolucaoEdgeSet *s){
	return arquivo.getPositionCount(*sl)<arquivo.getPositionCount(*s);
}
//belong é true se 's' nao for dominada por ninguém em arc
bool belong(BoundedParetoSet &arquivo, SolucaoEdgeSet* s){
	list<SolucaoEdgeSet* > solucoes = arquivo.getElementos();
	for (list<SolucaoEdgeSet* >::iterator it = solucoes.begin(); it!=solucoes.end(); it++){
		// caso alguém domine s, retorna false
		if (*(*it) >> *s) return false;
		//if ((*it).second.first<=s.second.first && (*it).second.second<=s.second.second && ((*it).second.first<s.second.first || (*it).second.second<s.second.second)) return false;
	}
	return true;
}

void include(int origem, int destino, list <tabuItem> &tabu_list){
	bool ha = false;
	for (list <tabuItem> ::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
		if (((*it).origem==origem && (*it).destino==destino) || ((*it).origem==destino && (*it).destino==origem)){
			ha = true;
			break;
		}
	}
	if (ha==false){
		tabuItem item = {origem, destino, TABUTENURE};
		tabu_list.push_back(item);
	}
}

bool isTabu(int origem, int destino, list <tabuItem> &tabu_list){
	for (list <tabuItem> ::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
		if (((*it).origem==origem && (*it).destino==destino) || ((*it).origem==destino && (*it).destino==origem)){
			return true;
		}
	}
	return false;
}

/*a busca tabu edita a solucao 's' que foi passada como argumento*/
bool buscaTabu(SolucaoEdgeSet *s, double lambda[NUMOBJETIVOS]){
	bool ret = false;
	BoundedParetoSet local_arc;
	list<SolucaoEdgeSet *> arqq  = arc_global.getElementos();
	list<SolucaoEdgeSet *>::iterator it = arqq.begin();
	while (it!=arqq.end()){
		if ((*(*it) >> *s)==false){
			local_arc.adicionarSol((*it));
		}
		it++;
	}
	list <tabuItem> tabu_list;
	int contIteracoes = 0;
	bool s_was_modified = false;
	//int a1, a2; // arestas que serao trocadas
	int origem_a1, destin_a1;
	int r; // do pseudo-codigo
	SolucaoEdgeSet *s_linha = new SolucaoEdgeSet(NUMEROVERTICES-1);
	//cout<<s->getObj(0)<<" "<<s->getObj(1)<<endl;
	do{ // repita até contIteracoes==max_tabu or s nao seja modificado
		s_was_modified = false;
		r = 0;
		do{	
			r++;
			int e = s_linha->getVizinho1(NUMEROVERTICES-1-1,*s, lambda);
			origem_a1 = s_linha->edges[e][0];
			destin_a1 = s_linha->edges[e][1];
			// vizinhança
			if (*s_linha >> *s) {
				*s = *s_linha;
				s_was_modified = true;
				ret = true;
			} else if (isTabu(origem_a1, destin_a1, tabu_list)==false && 
				m_grid(local_arc, s_linha, s)==true){ //isTabu(origem_a2, destin_a2, tabu_list)==false  && 

				*s = *s_linha;
				s_was_modified = true;
			}
			//s_linha->isTree();
			//cout<<s_linha->getObj(0)<<" "<<s_linha->getObj(1)<<endl;
		} while (r<MAX_VIZINHOS && s_was_modified == false);
		for (list <tabuItem>::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
			(*it).tempo -=1;
			if ((*it).tempo==0) {
				it = tabu_list.erase(it);
				if (it!=tabu_list.begin() && it!=tabu_list.end()) it--;
			}
		}
		if (s_was_modified == true){
			//cout<<s->getObj(0)<<" "<<s->getObj(1)<<endl;
			include(origem_a1, destin_a1, tabu_list);
			//include(origem_a2, destin_a2, tabu_list);
			if (arc_global.adicionarSol(s)==true) ret = true;
			local_arc.adicionarSol(s);
		}
		contIteracoes++;
	}while (contIteracoes<MAX_TABU && s_was_modified == true);
//	cout<<"contIteracoes = "<<contIteracoes<<endl;
	return ret;
	//delete[] s_aux.first; 
}

#endif
