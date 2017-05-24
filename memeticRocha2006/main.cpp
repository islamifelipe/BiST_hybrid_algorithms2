/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements the original memetic algorithm of Rocha et al (2006)
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
Aresta ** arestasPtr;
struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
Arquivo global_arc;


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
		int sizeAntesGlobalArc = global_arc.getSize(); // usado para verificcar se o tamanho do arquivo muda
		cout<<"populacao.size() = "<<populacao.size()<<endl;
		for (int p = 0; p<populacao.size(); p++){
			Arquivo local;
			list<pair<int*, pair<float, float> > > globalSolucoes = global_arc.getSolucoes();
		
			//preenche local
			for (list<pair<int*, pair<float, float> > >::iterator it = globalSolucoes.begin(); it != globalSolucoes.end(); it++){
		   		if (((*it).second.first<=populacao[p].second.first && (*it).second.second<=populacao[p].second.second && ((*it).second.first<populacao[p].second.first || (*it).second.second<populacao[p].second.second))==false){
		   			local.adicionarSol(clone(g, (*it)));
		   		}
		   	}
		   	// cout<<populacao[p].second.first<<" "<<populacao[p].second.second<<endl;
		   	buscaTabu(g, populacao[p],&local,&global_arc);
		   	 // cout<<populacao[p].second.first<<" "<<populacao[p].second.second<<endl;
		   	 // cout<<endl;
		}
		vector< pair<int*, pair<float, float> > > NovaPopulacao ;
		for (int j = 0; j<size_pop; j++){
			int q = 0;
			pair<int*, pair<float, float> > filho = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
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
				//cout<<"\t \t aqui3"<<endl;
				bool rest = crossover(g, pai.first, mae.first, filho);
				//cout<<"\t \t aqui4"<<endl;
				if (rest == true){
					// if ((filho.second.first == pai.second.first && filho.second.second == pai.second.second)||(filho.second.first == mae.second.first && filho.second.second == mae.second.second)) continue; //cout<<"IGUAL"<<endl;
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
			NovaPopulacao.push_back(filho);
		}
		for (int copi = 0; copi<populacao.size(); copi++){ 
			delete[] populacao[copi].first; // // substitui a populacao completamente
		}
		populacao.clear();
		for (int copi = 0; copi<NovaPopulacao.size(); copi++){ 
			if (thereis(populacao,  NovaPopulacao[copi])==false){
				populacao.push_back( NovaPopulacao[copi]);
			}
		}
		NovaPopulacao.clear();
		
		if (sizeAntesGlobalArc == global_arc.getSize()) contMaxSemMudar++;
		if (contMaxSemMudar == max_it){
			contMaxSemMudar=0;
			diversify(g, populacao);
		}

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
	
	my_grafo.updateIndex(); // depois, atualizamos os idexes das arestas no map
	nA = my_grafo.getQuantArestas();
	arestas = my_grafo.get_allArestas();
	arestasPtr = my_grafo.getAllArestasPtr();


 	times(&tempsInit);
 	
	memeticoRocha2006(&my_grafo);
	//vector< pair<int*, pair<float, float> > > pop_inicial = getPopulacaoInicial(&my_grafo,global_arc);
	
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