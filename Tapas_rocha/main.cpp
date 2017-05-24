/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements the TAPAS (TALBI, 2015) algorithm based on memetic algorithm of Rocha et al (2006)
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
#include "margeSort.h"
#include <sys/times.h>
#include <unistd.h>
#include "Parametros.h"
#include "PopulacaoInicial.h"
#include "Arquivo.h"
#include "Tabu.h"
#include <stdio.h> 

using namespace std;

map <int, Aresta *> arestas;
//Aresta ** arestasPtr;
struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
Arquivo global_arc;

void diversify2(Grafo *g, pair<int*, pair<float, float> > &sol){
	vector <int> amostral;
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantVertices()-1; i++){
		if (g->getStatus(sol.first[i])==1){
			conjunto.union1(g->getArestas(sol.first[i])->getOrigem(),g->getArestas(sol.first[i])->getDestino());
		}
	}

	for (int i=0; i<g->getQuantVertices()-1; i++){
		if (g->getStatus(sol.first[i])==0 && conjunto.compare(g->getArestas(sol.first[i])->getOrigem(),g->getArestas(sol.first[i])->getDestino())==false){
			amostral.push_back(sol.first[i]);
		}
	}
	int idRand1 = amostral[rand()%amostral.size()];
	// int idRand2 = amostral[rand()%amostral.size()];
	// int idRand3 = amostral[rand()%amostral.size()];
	g->setStatus(idRand1, 1);
	// g->setStatus(idRand2, 1);
	// g->setStatus(idRand3, 1);
	
	pair<int*, pair<float, float> > novo = RandomWalk(g);
	

	g->setStatus(idRand1, 0);
	// g->setStatus(idRand2, 0);
	// g->setStatus(idRand3, 0);
	if (novo.second.first<=sol.second.first && novo.second.second<=sol.second.second && ( novo.second.first<sol.second.first || novo.second.second<sol.second.second)){
		for (int i=0; i<g->getQuantVertices()-1; i++) sol.first[i] = novo.first[i];
		sol.second.first = novo.second.first;
		sol.second.second = novo.second.second;
		global_arc.adicionarSol(novo); // nao precisa de clone
	}else if (m_grid(&global_arc, novo, sol)==true){
		for (int i=0; i<g->getQuantVertices()-1; i++) sol.first[i] = novo.first[i];
		sol.second.first = novo.second.first;
		sol.second.second = novo.second.second;
		global_arc.adicionarSol(novo); // nao precisa de clone
	}	
}

/*substitui 50% dos individeuos da populacao (e nao do arquivo externo) por individuos gerdos com o RandomWalk*/
void diversify(Grafo *g, vector< pair<int*, pair<float, float> > > &pop){
	for (int  i=0; i<size_pop/2; i++){
		pair<int*, pair<float, float> > tree = RandomWalk(g);
		int sai = rand()%pop.size();
		delete pop[sai].first; // Observaçao : como estamos deletando o individuo da populacao, entao é importante que caso ele também estivesse no arquivo, ele seja uma copia. 
		//Ou seja, um individuo esteja na populacao e no arquivo externo, se nao for copia (se for o ponteiro) isso vai dar erro
		pop[sai] = tree;
		global_arc.adicionarSol(clone(g, tree));
	}
}

/*Este operador crossover usado por Rocha el al (2006) é baseado naquele proposto por Raidl (2000) 
O procedimento consiste em unir as arestas do pai e da mae e gerar uma arvore aleatoria a partir desta uniao

pair<int*, pair<float, float> > filho = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
	
*/
bool crossover(Grafo *g, int* pai, int *mae, pair<int*, pair<float, float> > &filho){
	vector <int> uniao;
	filho.second.first = 0;
	filho.second.second = 0;
	Conjunto conjunto(g->getQuantVertices()-1);
	int cont=0;
	for (int pp=0; pp<g->getQuantVertices()-1; pp++){
		for(int mm=0; mm<g->getQuantVertices()-1; mm++){
			if (mae[mm]==pai[pp]){
				if (conjunto.compare(g->getArestas(pai[pp])->getOrigem(), g->getArestas(pai[pp])->getDestino())==false){
					conjunto.union1(g->getArestas(pai[pp])->getOrigem(), g->getArestas(pai[pp])->getDestino());
					filho.first[cont++] = pai[pp];
					filho.second.first  += g->getArestas(pai[pp])->getPeso1();
					filho.second.second += g->getArestas(pai[pp])->getPeso2();
				}
			}
		}
	}
	//cout<<"cont = "<<cont<<endl;
	for (int i=0; i<g->getQuantVertices()-1; i++){
		if (conjunto.compare(g->getArestas(pai[i])->getOrigem(), g->getArestas(pai[i])->getDestino())==false){
			uniao.push_back(pai[i]);
		}
		if (conjunto.compare(g->getArestas(mae[i])->getOrigem(), g->getArestas(mae[i])->getDestino())==false){
			uniao.push_back(mae[i]);
		}
	}

	while (cont<g->getQuantVertices()-1 && uniao.size()!=0){
		int index1 = rand()%uniao.size();
		int index2 = rand()%uniao.size();
		int id1 = uniao[index1];
		int id2 = uniao[index2];
		if (id1 != id2){
			int id = id2;
			int index = index2;
			if (g->getArestas(id1)->getPeso1()<=g->getArestas(id2)->getPeso1() && g->getArestas(id1)->getPeso2()<=g->getArestas(id2)->getPeso2() && ( g->getArestas(id1)->getPeso1()<g->getArestas(id2)->getPeso1()||g->getArestas(id1)->getPeso2()<g->getArestas(id2)->getPeso2())){
				id = id1;
				index = index1;
			}
			if (conjunto.compare(g->getArestas(id)->getOrigem(), g->getArestas(id)->getDestino())==false){
				conjunto.union1(g->getArestas(id)->getOrigem(), g->getArestas(id)->getDestino());
				filho.first[cont++] = id;
				filho.second.first  += g->getArestas(id)->getPeso1();
				filho.second.second += g->getArestas(id)->getPeso2();
			}
			uniao.erase(uniao.begin()+index); // O(n)
		} else uniao.erase(uniao.begin()+index1);
	}
	//if (cont==g->getQuantVertices()-1) return true;
	//cout<<"cont = "<<cont<<endl;
	while (cont<g->getQuantVertices()-1){
		int a;
		int index1 = rand()%g->getQuantArestas();
		int index2 = rand()%g->getQuantArestas();
		if (index1!=index2){
			if (g->getArestas(index1)->getPeso1()<=g->getArestas(index2)->getPeso1() && g->getArestas(index1)->getPeso2()<=g->getArestas(index2)->getPeso2() && ( g->getArestas(index1)->getPeso1()<g->getArestas(index2)->getPeso1() || g->getArestas(index1)->getPeso2()<g->getArestas(index2)->getPeso2())){
				a = index1;
			} else {
				a = index2;
			}
			if (conjunto.compare(g->getArestas(a)->getOrigem(), g->getArestas(a)->getDestino())==false){
				conjunto.union1(g->getArestas(a)->getOrigem(), g->getArestas(a)->getDestino());
				filho.first[cont++] = a;
				filho.second.first  += g->getArestas(a)->getPeso1();
				filho.second.second += g->getArestas(a)->getPeso2();
			}
		}
	}
	if (cont==g->getQuantVertices()-1) return true;
	return false;
}

void memeticoRocha2006(Grafo *g){
	vector< pair<int*, pair<float, float> > > populacao = getPopulacaoInicial(g,global_arc);
	int contMaxSemMudar = 0;
	for (int i=0; i<max_gen; i++){
		cout<<"geracao i = " <<i<<endl;
		cout<<"sizee = "<<global_arc.getSize()<<endl;
		for (int j = 0; j<populacao.size(); j++){
			float alfa = (rand()%1001)/1000.0 ;// 0 ... 1000
			if (alfaMutacao>alfa){
				diversify2(g, populacao[j]);
			}
		}
		vector< pair<int*, pair<float, float> > > NovaPopulacao ;
		for (int j = 0; j<size_pop; j++){
			int q = 0;
			pair<int*, pair<float, float> > filho = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
			int conttt = 0;
			do{
				int id1 = rand()%populacao.size();
				int id2 = rand()%populacao.size();
				int id3 = rand()%populacao.size();
				int id4 = rand()%populacao.size();
				pair<int*, pair<float, float> > p1 = populacao[id1];
				pair<int*, pair<float, float> > p2 = populacao[id2];
				pair<int*, pair<float, float> > p3 = populacao[id3];
				pair<int*, pair<float, float> > p4 = populacao[id4];
				pair<int*, pair<float, float> > pai, mae;
				//if (id1==id2 || id1==id3|| id1==id4 || id2==id3 || id2==id4 || id3==id4) continue; // pode tomar muito tempo do loop
				
				//torneios binarios
				if (p1.second.first<p2.second.first) pai = p1;
				else pai = p2;

				if (p3.second.second<p4.second.second) mae = p3;
				else mae = p4;
				bool rest = crossover(g, pai.first, mae.first, filho);
				if (rest == true){
					if ((filho.second.first == pai.second.first && filho.second.second == pai.second.second)||(filho.second.first == mae.second.first && filho.second.second == mae.second.second)){
						conttt++;
						if (conttt<=30)
							continue; //cout<<"IGUAL"<<endl;
					} 
					conttt=0;
					if(belong(&global_arc, filho)==true || (m_grid(&global_arc, filho, pai)==true && m_grid(&global_arc, filho, mae)==true)){
						q = max_offsp+1;
						global_arc.adicionarSol(clone(g,filho)); // aceita o filho e sai do do..while 
					} else {
						q++;
					}
				} else {
					q++;
				}
			}while (q<max_offsp);
			if (q == max_offsp){
				list<pair<int*, pair<float, float> > > globalSolucoes = global_arc.getSolucoes();
				int novoRadom = rand()%globalSolucoes.size();
				int conttt =0;
				list<pair<int*, pair<float, float> > >::iterator it = globalSolucoes.begin();
				for (; it != globalSolucoes.end() && conttt<novoRadom; it++, conttt++);
				for (int ooo = 0; ooo<g->getQuantVertices()-1; ooo++) filho.first[ooo] = (*it).first[ooo];
				filho.second.first  =  (*it).second.first;
				filho.second.second =  (*it).second.second;
				//cout<<"(*it).first[0] = "<<(*it).first[0]<<endl;
			}
			if (thereis(NovaPopulacao, filho)==false){
				NovaPopulacao.push_back(filho);
			} else { 
				diversify2(g,filho);
				NovaPopulacao.push_back(filho);
				
			}
		}
		for (int copi = 0; copi<populacao.size(); copi++){ // substitui a populacao completamente
			if (copi<NovaPopulacao.size()){
				delete[] populacao[copi].first;
				populacao[copi] = NovaPopulacao[copi];
			}

			Arquivo local;
			list<pair<int*, pair<float, float> > > globalSolucoes = global_arc.getSolucoes();
		
			//preenche local
			for (list<pair<int*, pair<float, float> > >::iterator it = globalSolucoes.begin(); it != globalSolucoes.end(); it++){
		   		if (((*it).second.first<=populacao[copi].second.first && (*it).second.second<=populacao[copi].second.second && ((*it).second.first<populacao[copi].second.first || (*it).second.second<populacao[copi].second.second))==false){
		   			local.adicionarSol(clone(g, (*it)));
		   		}
		   	}
		   	
		   	 //cout<<populacao[copi].second.first<<" "<<populacao[copi].second.second<<endl;
		   	buscaTabu(g, populacao[copi],&local,&global_arc);
		   	 // cout<<populacao[copi].second.first<<" "<<populacao[copi].second.second<<endl;
		   	 // cout<<endl;
		}
		
	}	
	for (int p = 0; p<populacao.size(); p++){
		delete[] populacao[p].first;
	}


}

/*retorna a distância euclidiana entre dois pontos*/
float distancia(pair<float, float> s1, pair<float, float> s2){
	return sqrt((s1.first - s2.first)*(s1.first - s2.first) + (s1.second - s2.second)*(s1.second - s2.second));
}


/* 	
	uma arvore s' é vizinha de s sse elas diferem em somente uma aresta
	retorna os vizinhos de "s" que estao na regiao As e And(Talbi, 2013). 
	Ou seja, retorna os vizinhos que dominam extritamente "s" e que estao mais poximos do target.
	Os vizinhos em And (vizinhos incomparaveis com "s", sao adicionandos no aquivo externo)
*/
vector <pair<int*,  pair<float, float> > > vizinhos(Grafo *g, pair<int*, pair<float, float> > sol, float distanciaOriginal, pair<float, float> target){
	vector <pair<int*,  pair<float, float> > > retorno;
	for (int a = 0; a<g->getQuantVertices()-1; a++){ // n-1 (aresta que sairá)
		int dof = (rand()%2);
		if (g->getStatus(sol.first[a])==0 && dof==1){ // so sai se for opcional. Se for obrigatoria, nao sai
			int idArestaSai = sol.first[a];
			Conjunto conjunto(g->getQuantVertices());
			for (int i = 0; i <g->getQuantVertices()-1; i++){
				if (i!=a){
					if (sol.first[i]<0 || sol.first[i]>g->getQuantArestas()-1) cout<<"ERROOOO sol.first[i] = "<<sol.first[i]<<endl;
					conjunto.union1(g->getArestas(sol.first[i])->getOrigem(), g->getArestas(sol.first[i])->getDestino());	
				}
			}
		//	cout<<"FIM AQUI 1"<<endl;
			float peso1base = sol.second.first - g->getArestas(sol.first[a])->getPeso1();
			float peso2base = sol.second.second - g->getArestas(sol.first[a])->getPeso2();
			
		 	for (int i = 0; i <g->getQuantArestas(); i++){
		 		int odfff=(rand()%2);
		 		if (i!=idArestaSai && odfff==1){
		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
		 				float newPeso1 = peso1base+g->getArestas(i)->getPeso1();
		 				float newPeso2 = peso2base+g->getArestas(i)->getPeso2();
		 				// se o vizinho nao é fracamente dominado por "s", insere no conjunto
		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
		 					int *fantasma;
		 					// (m_grid(&global_arc, make_pair(fantasma, make_pair(newPeso1, newPeso2)), sol) == true)
		 					if ( (newPeso1 < sol.second.first &&  newPeso2 < sol.second.second) ){ // ||  (newPeso1 < sol.second.first &&  newPeso2 <= sol.second.second) ||  (newPeso1 <= sol.second.first &&  newPeso2 < sol.second.second)){ // se for extritamente dominado, entao está As
		 						int *base = new int[g->getQuantVertices()-1];
			 					for (int are = 0; are <g->getQuantVertices()-1; are++){
			 						if (are == a) base[are] = i;
			 						else base[are] = sol.first[are];
			 					} 
			 					pair<int*,  pair<float, float> > fknrjgnj = make_pair(base, make_pair(newPeso1,newPeso2));
			 					global_arc.adicionarSol(clone(g,fknrjgnj));
			 					if (distancia(fknrjgnj.second, target)<distanciaOriginal){ // retornar apenas vizinhos que dominanm extritamente 's' e que estao mais proximos do target, ou seja, que a distância euclidiana entre o vizinho e o target é menor que a distância entre 's' e o target
			 						retorno.push_back(fknrjgnj); // adiciona no conjunto de retorno apenas se estiver em As
		 						} else {
		 							delete[] fknrjgnj.first;
		 						}
		 					} else {
		 						//se o novo vizinho nao dominar extritamente s e, mas dominar fracamente mesmo assim,  entao s e o novo vizinho sao incomparaveis
		 						int *base = new int[g->getQuantVertices()-1];
			 					for (int are = 0; are <g->getQuantVertices()-1; are++){
			 						if (are == a) base[are] = i;
			 						else base[are] = sol.first[are];
			 					} 
			 					pair<int*,  pair<float, float> > fknrjgnj = make_pair(base, make_pair(newPeso1,newPeso2));
			 					bool lfgk = global_arc.adicionarSol(fknrjgnj); // sem copia
			 					
			 				} 
		 				}  
		 			}
		 		} 
		 	}
		}
	}
	// cout<<"conrjrkgkr2 = "<<conrjrkgkr2<<endl;
	// cout<<"conrjrkgkr = "<<conrjrkgkr<<endl;
	return retorno;
}


bool compare(pair<int*, pair<float, float> > s1, pair<int*, pair<float, float> > s2){
	return s1.second.first<s2.second.first || (s1.second.first==s2.second.first && s1.second.second<=s2.second.second);
}

void path_relinking(Grafo *g, pair<int*, pair<float, float> > start, pair<float, float>  target){
	float distanciaOriginal = 0;
	int contMax = 0;
	pair<int*, pair<float, float> > startaux = clone(g, start);
	int sizeee = global_arc.getSize();
	int contttt=0;
	do{
		cout<<"\t \t contMax = "<<contMax<<endl;
		distanciaOriginal = distancia(startaux.second, target);
		//cout<<"\t \t distanciaOriginal = "<<distanciaOriginal<<endl;
		if (distanciaOriginal>0){
			//vizinhos que dominam extritamente 'startaux' e que se aproximam do target
			vector <pair<int*,  pair<float, float> > > vizi =  vizinhos(g, startaux, distanciaOriginal, target);
			if (vizi.size()>0){

				pair<int*, pair<float, float> > proximooo = vizi[rand()%vizi.size()]; // sortea o proximo a ser verificado
				contMax++;
				for (int i=0; i<g->getQuantVertices(); i++) startaux.first[i] = proximooo.first[i];
				startaux.second.first = proximooo.second.first;
				startaux.second.second = proximooo.second.second;
			} else {
				contMax= maxPR+1; // sai do laço
			}

			if (global_arc.getSize()==sizeee) {
				contttt++;
				if (contttt == 3) contMax = maxPR+1; // 3 = max_it
			}
			else {
				sizeee = global_arc.getSize();
				contttt = 0;
			}
		}
	} while (distanciaOriginal > 0 && contMax<maxPR);
	delete[] startaux.first;


}

/*recebe a lista de pontos (aproximativos) ordenados lexicograficamente*/
void tapas(Grafo *g){
	/*dado um ponto (aproximativo) si, o gi correspondente é gi = (x_s(i-1), y_s(i+1))*/
	list<pair<int*, pair<float, float> > > pontos = global_arc.getSolucoes();
	/*ATENCAO : precisa-se fazer um clone de cada ponto aproximativo (entrada do tapas e saida do memético),
	porque um desses pontos pode ser deletado no arquivo (pois se torna dominado), e perderíamos portanto a referência da lista de aresta que compoe tal arvore*/
		
	pontos.sort(compare);
	vector<pair<int*, pair<float, float> > > copiaPontos;
	for (list<pair<int*, pair<float, float> > >::iterator it = pontos.begin(); it!=pontos.end() ;  it++){
		copiaPontos.push_back(clone(g, (*it)));
	}
	// for (int cont = 1; cont<copiaPontos.size()-1; cont++){
	// 	//if ()
	// 	cout<<"cont = "<<cont<<endl;
	// 	pair<float, float> ant = copiaPontos[cont-1].second;

	// 	pair<float, float> pos= copiaPontos[cont+1].second;

	// 	pair<float, float> gi = make_pair(ant.first, pos.second);

	// 	path_relinking(g, copiaPontos[cont], gi); // origem e target
	// 	cout<<"global_arc.getSize() = "<<global_arc.getSize()<<endl;
	// }

	for (int cont=0; cont<maxParetoAproximativo && global_arc.getSize()<maxSolucoes && copiaPontos.size()>2; cont++){
		int max = copiaPontos.size()-2; // entre 0 e copiaPontos.size()-3
		int index = rand()%(max)+1; // entre 1 e copiaPontos.size()-2
		cout<<"Ponto  = "<<index<<endl;
		cout<<"cont  = "<<cont<<endl;

		pair<float, float> ant = copiaPontos[index-1].second;
		pair<float, float> pos = copiaPontos[index+1].second;

		pair<float, float> gi = make_pair(ant.first, pos.second);
		path_relinking(g, copiaPontos[index], gi); // origem e target
		cout<<"global_arc.getSize() = "<<global_arc.getSize()<<endl;

		copiaPontos.erase(copiaPontos.begin()+index);
	
	}
}

/*	 Deve receber dois arquivos como parâmetro. 
	argv[1] arquivo em que será guardado a fronteira de pareto
	argv[2] arquivo em que será guardado tempo
*/
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
	//arestasPtr = my_grafo.getAllArestasPtr();
	//my_grafo.computaPreSorting();
	//preSorting = my_grafo.getPreSorting();
	

 	times(&tempsInit);
 	
	memeticoRocha2006(&my_grafo);
	cout<<"Fim da primeira fase. Size = "<<global_arc.getSize()<<endl;
   	tapas(&my_grafo);
   	cout<<"Fim da segunda fase. Size = "<<global_arc.getSize()<<endl;
   	
	//vector< pair<int*, pair<float, float> > > populacao = getPopulacaoInicial(&my_grafo,global_arc);
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<" segundos"<<endl;//"Tempo do usuario por segundo : "
   	

   	FILE *paretoFront = fopen(argv[1],"a");
   	FILE *tempofile = fopen(argv[2],"a");
   

   	fprintf(tempofile,"%.10lf\n", (float) user_time1 / (float) sysconf(_SC_CLK_TCK));
   	
   	int i = 0;
   	cout<<"Resultado : "<<endl;
   	list<pair<int*, pair<float, float> > > solucess = global_arc.getSolucoes();
   	for (list<pair<int*, pair<float, float> > >::iterator it = solucess.begin(); it != solucess.end(); it++){
   		float cont1=0, cont2=0;
		pair<int*, pair<float, float> > arvr = (*it);
		//cout<<"Arvore "<<i+1<<endl;
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
    	i++;
    	cout<<arvr.second.first<<" "<<arvr.second.second<<endl;
    	fprintf(paretoFront,"%.10lf %.10lf\n", arvr.second.first, arvr.second.second);
   	
    	if (cont1 != arvr.second.first || cont2 != arvr.second.second) {
    		cout<<"ERROROFKROKFORKFORKF 3"<<endl;
    	}
	
   	}

   	fprintf(paretoFront,"\n");
   	fclose(paretoFront);
   	fclose(tempofile);


	return 0;
}