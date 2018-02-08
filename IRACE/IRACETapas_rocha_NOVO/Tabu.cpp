
#include "Tabu.h"

typedef struct {
	Aresta *aresta;
	int tempo; /*inicialmente é tabutenure. Ai vai diminuindo a cada iteraçao. Quando chegar em zero, sai*/ 
} tabuItem;

/* pre-condicao : aresta i nao pode ser obrigatoria
	retorna false caso nao seja possivel fazer a troca das arestas (pois o grafo pode nao ser completo)
// id_i , id_j = index na arvore
*/
bool troca(Grafo *g, pair<int*, pair<float, float> > &s, int id_i,  int id_j){
	if (g->getStatus(s.first[id_i])==1 || g->getStatus(s.first[id_j])==1) return false;
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantVertices()-1; i++){
		int id = (s.first)[i];
		if (id != s.first[id_i] && id != s.first[id_j]){
			conjunto.union1(g->getArestas(id)->getOrigem(), g->getArestas(id)->getDestino());
		}
	}
	vector<int> amostral;
	for (int i=0; i<g->getQuantArestas(); i++){
		if (i != s.first[id_i] && i != s.first[id_j]){
			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
				amostral.push_back(i);
			}	
		}
	}
	if (amostral.size()== 0) return false;


	int ram = rand()%amostral.size();
	int idescolhido = amostral[ram];
	amostral.erase(amostral.begin()+ram);
	s.second.first = s.second.first - g->getArestas(s.first[id_i])->getPeso1() + g->getArestas(idescolhido)->getPeso1();
	s.second.second = s.second.second - g->getArestas(s.first[id_i])->getPeso2() + g->getArestas(idescolhido)->getPeso2();
	s.first[id_i] = idescolhido;
	conjunto.union1(g->getArestas(idescolhido)->getOrigem(), g->getArestas(idescolhido)->getDestino());
	

	bool achou = false;
	do {
		if (amostral.size() > 0){
			int ram = rand()%amostral.size();
			int idescolhido = amostral[ram];
			amostral.erase(amostral.begin()+ram);
			if (conjunto.compare(g->getArestas(idescolhido)->getOrigem(), g->getArestas(idescolhido)->getDestino())==false){
				s.second.first = s.second.first - g->getArestas(s.first[id_j])->getPeso1() + g->getArestas(idescolhido)->getPeso1();
				s.second.second = s.second.second - g->getArestas(s.first[id_j])->getPeso2() + g->getArestas(idescolhido)->getPeso2();
				s.first[id_j] = idescolhido;
				conjunto.union1(g->getArestas(idescolhido)->getOrigem(), g->getArestas(idescolhido)->getDestino());
				achou=true;
			}
		}
	} while (amostral.size() > 0 && achou!=true);
	if (achou == false) return false;
	return true;
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
	
	tabuItem item = {aresta, tabutenure};
	tabu_list.push_back(item);

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
	//pair<int*, pair<float, float> > s_aux = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0)); 
	do{ // repita até contIteracoes==max_tabu or s nao seja modificado
		
		s_was_modified = false;
		int r = 0;
		int index_i,index_j; // é index da arvore. Representa a aresta que sai da arvore (passados ao procedimento troca)
		//depois de chamar o procedimento "troca", este index contém a aresta que entrou
		int id_i, id_j; // auxiliar pra guardar a aresta de index_i e index_j antes da chamada de troca
		vector<int> amostral;
		for (int i=0; i<g->getQuantVertices()-1; i++){
			if(g->getStatus(s.first[i]) ==0){ // podem ser sorteadas pra sair apenas arestas opcionais
				amostral.push_back(i);
			}
		}
		do{	
			
			if (amostral.size()==0) break;

			r++;
			int rnam = rand()%(amostral.size());
			index_i = amostral[rnam]; // seleciona a primeira aresta que vai sair (seleciona index da aresta na arvore)
			amostral.erase(amostral.begin()+rnam);
			id_i = g->getArestas(s.first[index_i])->getId(); //
			
			if (amostral.size()==0) break; // AQEFYUUIUFF
			
			rnam = rand()%(amostral.size());
			index_j = amostral[rnam]; // seleciona a segunda aresta que vai sair (seleciona index da aresta na arvore)
			amostral.erase(amostral.begin()+rnam);
			id_j = g->getArestas(s.first[index_j])->getId(); //

			float before1 =  s.second.first;
			float before2 = s.second.second;
			bool trocou = troca(g, s, index_i, index_j); // s ja sai modificado
			if (trocou == true){
				
				if (s.second.first<=before1 && s.second.second<=before2 && (s.second.first<before1 || s.second.second<before2)){
					
					s_was_modified = true;
				} 
				else {
					int *fantasma;
					if (isTabu(g->getArestas(id_i), tabu_list)==false && isTabu(g->getArestas(id_j), tabu_list)==false &&
					m_grid(local_arc, s, make_pair(fantasma, make_pair(before1, before2)))==true){
						
						s_was_modified = true;
					} else {
						s.first[index_i] = id_i; // recoloca a aresta
						s.first[index_j] = id_j; // recoloca a aresta
						s.second.first = before1;
						s.second.second = before2;
					}
				}
			} 
		} while (r<max_neighbors && s_was_modified == false);
		
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
		for (list <tabuItem> ::iterator it = tabu_list.begin(); it!=tabu_list.end(); it++){
			(*it).tempo -=1;
			if ((*it).tempo==0) {
				it = tabu_list.erase(it);
				if (it!=tabu_list.begin() && it!=tabu_list.end()) it--;
			}
		}
		
	}while (contIteracoes<max_tabu && s_was_modified == true);
	//cout<<"contIteracoes = "<<contIteracoes<<endl;
	
	//delete[] s_aux.first; // TODO: poderia deixar s_aux como global e evitar alocar e desalocar a cada chamada da busca tabu
}