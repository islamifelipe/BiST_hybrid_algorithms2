#include "Tabu.h"

typedef struct {
	Aresta *aresta;
	int tempo; /*inicialmente é tabutenure. Ai vai diminuindo a cada iteraçao. Quando chegar em zero, sai*/ 
} tabuItem;


/* pre-condicao : origens e destinos de cada aresta i e j sao mutualmente distintas
	retorna false caso nao seja possivel fazer a troca das arestas (pois o grafo pode nao ser completo)
// id_i e id_j = index's na arvore
*/
bool troca(Grafo *g, pair<int*, pair<float, float> > &s, int id_i, int id_j){
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantVertices()-1; i++){
		int id = (s.first)[i];
		if (id != s.first[id_i] && id != s.first[id_j]){
			conjunto.union1(g->getArestas(id)->getOrigem(), g->getArestas(id)->getDestino());
		}
	}
	int a  = g->getArestas(s.first[id_i])->getOrigem(); // a
	int b = g->getArestas(s.first[id_i])->getDestino(); // b
	int c  = g->getArestas(s.first[id_j])->getOrigem(); // c
	int d = g->getArestas(s.first[id_j])->getDestino(); // d
	//cout<<"Sai id_i = "<<s.first[id_i]<<" and id_j = "<<s.first[id_j]<<endl;
	if (g->getArestas(a, c)!=NULL && g->getArestas(b, d)!=NULL){
		// junta-se ac e bd. Caso forme ciclo, é porque ha mais de uma componente conexa. entao insere-se ad e bc
		conjunto.union1(a, c);
		conjunto.union1(b, d);

		//agora verifica se estao todas na mesma componente conexa. Se nao, inere-se ad e bc
		if (conjunto.compare(a, b)==true && conjunto.compare(a, c)==true && conjunto.compare(a, d)==true){
			s.second.first  = s.second.first  - g->getArestas((s.first)[id_i])->getPeso1() - g->getArestas((s.first)[id_j])->getPeso1() + g->getArestas(a, c)->getPeso1() + g->getArestas(b, d)->getPeso1();
			s.second.second = s.second.second - g->getArestas((s.first)[id_i])->getPeso2() - g->getArestas((s.first)[id_j])->getPeso2() + g->getArestas(a, c)->getPeso2() + g->getArestas(b, d)->getPeso2();
			(s.first)[id_i] = g->getArestas(a, c)->getId();
			(s.first)[id_j] = g->getArestas(b, d)->getId();
			//cout<<"entra id_i = "<<s.first[id_i]<<" and id_j = "<<s.first[id_j]<<endl;
			return true;
		} else if (g->getArestas(a, d)!=NULL && g->getArestas(b, c)!=NULL){
			// se nao, insere-se ad e bc
			s.second.first  = s.second.first  - g->getArestas((s.first)[id_i])->getPeso1() - g->getArestas((s.first)[id_j])->getPeso1() + g->getArestas(a, d)->getPeso1() + g->getArestas(b, c)->getPeso1();
			s.second.second = s.second.second - g->getArestas((s.first)[id_i])->getPeso2() - g->getArestas((s.first)[id_j])->getPeso2() + g->getArestas(a, d)->getPeso2() + g->getArestas(b, c)->getPeso2();
			
			(s.first)[id_i] = g->getArestas(a, d)->getId();
			(s.first)[id_j] = g->getArestas(b, c)->getId();
			//cout<<"entra id_i = "<<s.first[id_i]<<" and id_j = "<<s.first[id_j]<<endl;
	
			return true;
		}

		return false;

	} else if (g->getArestas(a, d)!=NULL && g->getArestas(b, c)!=NULL) {
		// junta-se ad e bc. Caso forme ciclo, retorna false. Se nao, insere
		conjunto.union1(a, d);
		conjunto.union1(b, c);

		//agora verifica se estao todas na mesma componente conexa. Se nao, retorna false
		if (conjunto.compare(a, b)==true && conjunto.compare(a, c)==true && conjunto.compare(a, d)==true){
			s.second.first  = s.second.first  - g->getArestas((s.first)[id_i])->getPeso1() - g->getArestas((s.first)[id_j])->getPeso1() + g->getArestas(a, d)->getPeso1() + g->getArestas(b, c)->getPeso1();
			s.second.second = s.second.second - g->getArestas((s.first)[id_i])->getPeso2() - g->getArestas((s.first)[id_j])->getPeso2() + g->getArestas(a, d)->getPeso2() + g->getArestas(b, c)->getPeso2();
			
			(s.first)[id_i] = g->getArestas(a, d)->getId();
			(s.first)[id_j] = g->getArestas(b, c)->getId();
			//cout<<"entra id_i = "<<s.first[id_i]<<" and id_j = "<<s.first[id_j]<<endl;
	
			return true;
		}
		return false;
	}
	return false;
}

//retorna “verdadeiro” caso sl esteja em uma regiao menos populosa que s com relacao ao arquivo local_arc.
bool m_grid(Arquivo *local_arc, pair<int*, pair<float, float> > sl, pair<int*, pair<float, float> > s){
	return local_arc->getPositionCount(sl)<local_arc->getPositionCount(s);
}
//belong é true se 's' nao for dominada por ninguém em arc
bool belong(Arquivo *arq, pair<int*, pair<float, float> > s){
	list<pair<int*, pair<float, float> > > solucoes = arq->getSolucoes();
	for (list<pair<int*, pair<float, float> > >::iterator it = solucoes.begin(); it!=solucoes.end(); it++){
		// caso alguém domine s, retorna false
		if ((*it).second.first<=s.second.first && (*it).second.second<=s.second.second && ((*it).second.first<s.second.first || (*it).second.second<s.second.second)) return false;
	}
	return true;
}

void include(Aresta *aresta, list <tabuItem> &tabu_list){
	bool ha = false;
	for (list <tabuItem> ::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
		if ((*it).aresta->getId()==aresta->getId()) {
			ha = true;
			break;
		}
	}
	if (ha==false){
		tabuItem item = {aresta, tabutenure};
		tabu_list.push_back(item);
	}
}

bool isTabu(Aresta *aresta, list <tabuItem> &tabu_list){
	for (list <tabuItem> ::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
		if ((*it).aresta->getId()==aresta->getId()) {
			return true;
		}
	}
	return false;
}

/*a busca tabu edita a solucao 's' que foi passada como argumento*/
void buscaTabu(Grafo *g, pair<int*, pair<float, float> > &s, Arquivo *local_arc,Arquivo *global_arc){
	list <tabuItem> tabu_list;
	int contIteracoes = 0;
	bool s_was_modified = false;
	pair<int*, pair<float, float> > s_aux = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
	do{ // repita até contIteracoes==max_tabu or s nao seja modificado
		
		s_was_modified = false;
		int r = 0;
		int index_i, index_j; // sao index's da arvore. Representam as arestas que saem da arvore (passados ao procedimento troca)
		//depois de chamar o procedimento "troca", estes indexes contêm as arestas que entraram
		int id_i, id_j; // auxiliares pra guardar as arestas de index_i e index_j antes da chamada de troca
		// int contMAXXXX = 0;
		do{	
			// uma copia auxiliar de s
			for (int i=0; i<g->getQuantVertices()-1; i++)s_aux.first[i] = s.first[i];
			s_aux.second.first = s.second.first;
			s_aux.second.second = s.second.second;
			r++;
			index_i = rand()%(g->getQuantVertices()-1); // seleciona a primeira aresta que vai sair (seleciona index da aresta na arvore)
			vector<int> amostral;
			for (int i=0; i<g->getQuantVertices()-1; i++){ // pega todas as arestas tais que ...
				//if (i==0) cout<<"s_aux.first[i] = "<<s_aux.first[i]<<endl;
				if (g->getArestas(s_aux.first[i])->getOrigem() !=g->getArestas(s_aux.first[index_i])->getOrigem()  &&
					g->getArestas(s_aux.first[i])->getOrigem() !=g->getArestas(s_aux.first[index_i])->getDestino() &&
					g->getArestas(s_aux.first[i])->getDestino()!=g->getArestas(s_aux.first[index_i])->getOrigem()  &&
					g->getArestas(s_aux.first[i])->getDestino()!=g->getArestas(s_aux.first[index_i])->getDestino()){
						amostral.push_back(i); // index na arvore
				}
			}
			if (amostral.size()>0) {
				index_j = amostral[rand()%(amostral.size())]; // index na arvore
				// contMAXXXX=0;
			} else break;
			// else {
			// 	// // contMAXXXX++;
			// 	// // if (contMAXXXX<1000) {
			// 	// 	r--;
			// 	// // }
			// 	// continue;
			// }
			
			id_i = g->getArestas(s_aux.first[index_i])->getId(); //
			id_j = g->getArestas(s_aux.first[index_j])->getId();
			//cout<<s_aux.second.first<<" "<<s_aux.second.second<<endl;
			bool trocou = troca(g, s_aux, index_i, index_j);
			if (trocou == true){
				// contMAXXXX = 0;
				// cout<<s.second.first<<" "<<s.second.second<<endl;
				// cout<<s_aux.second.first<<" "<<s_aux.second.second<<endl;
				// cout<<endl;
				if (s_aux.second.first<=s.second.first && s_aux.second.second<=s.second.second && (s_aux.second.first<s.second.first || s_aux.second.second<s.second.second)){
					for (int i=0; i<g->getQuantVertices()-1; i++)s.first[i] = s_aux.first[i];
					s.second.first = s_aux.second.first;
					s.second.second = s_aux.second.second;
					s_was_modified = true;
				} 
				else if (isTabu(g->getArestas(id_i), tabu_list)==false && 
					isTabu(g->getArestas(id_j), tabu_list)==false && 
					m_grid(local_arc, s_aux, s)==true){
					for (int i=0; i<g->getQuantVertices()-1; i++)s.first[i] = s_aux.first[i];
					s.second.first = s_aux.second.first;
					s.second.second = s_aux.second.second;
					s_was_modified = true;
				}
			} else {
				// contMAXXXX++;
				// if (contMAXXXX<1000) {
				// 	r--;
				// // }
				// continue;
			}
		} while (r<max_neighbors && s_was_modified == false);
		for (list <tabuItem> ::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
			(*it).tempo -=1;
			if ((*it).tempo==0) {
				it = tabu_list.erase(it);
				if (it!=tabu_list.begin() && it!=tabu_list.end()) it--;
			}
		}
		if (s_was_modified == true){
			include(g->getArestas(id_i), tabu_list);
			//cout<<"Modificado : "<<s.second.first<<" "<<s.second.second<<endl;
			//cout<<"Entra tabu = "<<s.first[index_i]<<" and "<<s.first[index_j]<<endl;
			//cout<<"Saiu da    = "<<id_i<<"  and  "<<id_j<<endl;
			include(g->getArestas(id_j), tabu_list);
			
			//cout<<"insere em local"<<endl;
			local_arc->adicionarSol(clone(g,s)); // TODO: s talvez nao seja incluida no arquivo. Mas mesmo assim, o clone sera criado. Melhor colocar o clone dentre do if que garante a inserçao da solucao no arquivo

			//cout<<"insere em global"<<endl;
			global_arc->adicionarSol(clone(g,s)); 
		}
		contIteracoes++;
		
	}while (contIteracoes<max_tabu && s_was_modified == true);
	//cout<<"contIteracoes = "<<contIteracoes<<endl;
	
	delete[] s_aux.first; 
}
