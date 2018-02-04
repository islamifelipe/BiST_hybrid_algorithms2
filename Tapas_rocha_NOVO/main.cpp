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

//Aresta ** arestasPtr;
struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
Arquivo global_arc;
int quantidadeObrigatorias = 0;
pair<int*, pair<float, float> > obrigatorias;

void diversify2(Grafo *g, pair<int*, pair<float, float> > &sol){
	vector <int> amostral;
	Conjunto conjunto(g->getQuantVertices());
	for (int i=0; i<g->getQuantVertices()-1 && quantidadeObrigatorias>0 ; i++){
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
	g->setStatus(idRand1, 1);
	
	pair<int*, pair<float, float> > novo = RandomWalk(g, quantidadeObrigatorias);
	

	g->setStatus(idRand1, 0);
	if (novo.second.first<=sol.second.first && novo.second.second<=sol.second.second && ( novo.second.first<sol.second.first || novo.second.second<sol.second.second)){
		for (int i=0; i<g->getQuantVertices()-1; i++) sol.first[i] = novo.first[i];
		sol.second.first = novo.second.first;
		sol.second.second = novo.second.second;
		global_arc.adicionarSol(clone(g,novo)); // nao precisa de clone
	}else if (m_grid(&global_arc, novo, sol)==true){
		for (int i=0; i<g->getQuantVertices()-1; i++) sol.first[i] = novo.first[i];
		sol.second.first = novo.second.first;
		sol.second.second = novo.second.second;
		global_arc.adicionarSol(clone(g,novo)); // nao precisa de clone
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
	vector< pair<int*, pair<float, float> > > populacao = getPopulacaoInicial(g,global_arc, quantidadeObrigatorias);
	int contMaxSemMudar = 0;
	for (int i=0; i<max_gen; i++){
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
				NovaPopulacao.push_back(RandomWalk(g, quantidadeObrigatorias));
				
			}
		}
		for (int copi = 0; copi<populacao.size(); copi++){ // substitui a populacao completamente
			if (copi<NovaPopulacao.size()){
				delete[] populacao[copi].first;
				populacao[copi] = NovaPopulacao[copi];
			}
		}

		for (int copi = 0; copi<populacao.size(); copi++){ 
			Arquivo local;
			list<pair<int*, pair<float, float> > > globalSolucoes = global_arc.getSolucoes();
		
			//preenche local
			for (list<pair<int*, pair<float, float> > >::iterator it = globalSolucoes.begin(); it != globalSolucoes.end(); it++){
		   		if (((*it).second.first<=populacao[copi].second.first && (*it).second.second<=populacao[copi].second.second && ((*it).second.first<populacao[copi].second.first || (*it).second.second<populacao[copi].second.second))==false){
		   			local.adicionarSol(clone(g, (*it)));
		   		}
		   	}
		   	buscaTabu(g, populacao[copi],&local,&global_arc);
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




vector <pair<int*,  pair<float, float> > > vizinhos2(Grafo *g, pair<int*, pair<float, float> > sol, float distanciaOriginal, pair<float, float> target){
	vector <pair<int*,  pair<float, float> > > retorno; /*possiveis candidatos*/
	for (int viz=0; viz<maxVizinhos; viz++){
		int indexSair=rand()%(g->getQuantVertices()-1); // sorteia um index da arvore
		int idEscolhidaSair = sol.first[indexSair]; // pega o id da aresta correspondente
		if (g->getStatus(idEscolhidaSair)==0){
			Conjunto conjunto(g->getQuantVertices());
			for (int iii = 0; iii<g->getQuantVertices()-1; iii++){
				if (sol.first[iii]!=idEscolhidaSair){
					conjunto.union1(g->getArestas(sol.first[iii])->getOrigem(), g->getArestas(sol.first[iii])->getDestino());
				}
			}
			int peso1base = sol.second.first  - g->getArestas(idEscolhidaSair)->getPeso1();
			int peso2base = sol.second.second - g->getArestas(idEscolhidaSair)->getPeso2();
			///cout<<"Sai aresta id="<<idEscolhidaSair<<" Peso1 = "<<g->getArestas(idEscolhidaSair)->getPeso1()<<" Peso2 = "<<g.getArestas(idEscolhidaSair)->getPeso2()<<endl;
			vector<int> arestasPossiveis;
			for (int i = 0; i <g->getQuantArestas(); i++){
		 		if (i!=idEscolhidaSair){ // CORRIGIDO
		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
		 				int newPeso1 = peso1base+g->getArestas(i)->getPeso1();
		 				int newPeso2 = peso2base+g->getArestas(i)->getPeso2();
		 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
		 					arestasPossiveis.push_back(i);
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
				int newPeso1 = peso1base+g->getArestas(idEscolhidaEntra)->getPeso1();
		 		int newPeso2 = peso2base+g->getArestas(idEscolhidaEntra)->getPeso2();
				if ( (newPeso1 < sol.second.first &&  newPeso2 < sol.second.second)){ // TODO : m_grid?
					pair<int*,  pair<float, float> > fknrjgnj = make_pair(base, make_pair(newPeso1,newPeso2));
 					global_arc.adicionarSol(clone(g,fknrjgnj));
 					if (distancia(fknrjgnj.second, target)<distanciaOriginal){ // retornar apenas vizinhos que dominanm extritamente 's' e que estao mais proximos do target, ou seja, que a distância euclidiana entre o vizinho e o target é menor que a distância entre 's' e o target
 						retorno.push_back(fknrjgnj); // adiciona no conjunto de retorno apenas se estiver em As
					} else {
						delete[] fknrjgnj.first;
					}

				} else {
					pair<int*,  pair<float, float> > fknrjgnj = make_pair(base, make_pair(newPeso1,newPeso2));
			 		bool lfgk = global_arc.adicionarSol(clone(g,fknrjgnj)); // sem copia				
				}
			}
		} //else fora
	}
	return retorno;
}

bool compare(pair<int*, pair<float, float> > s1, pair<int*, pair<float, float> > s2){
	return s1.second.first<s2.second.first || (s1.second.first==s2.second.first && s1.second.second<s2.second.second);
}

void path_relinking(Grafo *g, pair<int*, pair<float, float> > start, pair<float, float>  target){
	float distanciaOriginal = 0;
	int contMax = 0;
	pair<int*, pair<float, float> > startaux = clone(g, start);
	int sizeee = global_arc.getSize();
	int contttt=0;
	do{
		//cout<<"\t \t contMax = "<<contMax<<endl;
		distanciaOriginal = distancia(startaux.second, target);
		//cout<<"\t \t distanciaOriginal = "<<distanciaOriginal<<endl;
		if (distanciaOriginal>0){
			//vizinhos que dominam extritamente 'startaux' e que se aproximam do target
			vector <pair<int*,  pair<float, float> > > vizi =  vizinhos2(g, startaux, distanciaOriginal, target);
			if (vizi.size()>0){

				pair<int*, pair<float, float> > proximooo = vizi[rand()%vizi.size()]; // sortea o proximo a ser verificado
				contMax++;
				for (int i=0; i<g->getQuantVertices(); i++) startaux.first[i] = proximooo.first[i];
				startaux.second.first = proximooo.second.first;
				startaux.second.second = proximooo.second.second;
			} else {
				contMax= maxPR+1; // sai do laço
			}
		}
	} while (distanciaOriginal > 0 && contMax<maxPR);
	delete[] startaux.first;


}

/*recebe a lista de pontos (aproximativos) ordenados lexicograficamente*/
void tapas(Grafo *g){
	/*dado um ponto (aproximativo) si, o gi correspondente é gi = (x_s(i-1), y_s(i+1))*/
	list<pair<int*, pair<float, float> > > pontos = global_arc.getSolucoes(); // maximo 300
	/*ATENCAO : precisa-se fazer um clone de cada ponto aproximativo (entrada do tapas e saida do memético),
	porque um desses pontos pode ser deletado no arquivo (pois se torna dominado), e perderíamos portanto a referência da lista de aresta que compoe tal arvore*/
		
	pontos.sort(compare);
	vector<pair<int*, pair<float, float> > > copiaPontos;
	for (list<pair<int*, pair<float, float> > >::iterator it = pontos.begin(); it!=pontos.end() ;  it++){
		copiaPontos.push_back(clone(g, (*it)));
	}

	for (int index=1; index<copiaPontos.size()-1; index++){
		
		pair<float, float> ant = copiaPontos[index-1].second;
		pair<float, float> pos = copiaPontos[index+1].second;

		pair<float, float> gi = make_pair(ant.first, pos.second);
		path_relinking(g, copiaPontos[index], gi); // origem e target
		//cout<<"global_arc.getSize() = "<<global_arc.getSize()<<endl;
	
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
	
	times(&tempsInit);

	my_grafo.excluiProibidas(); // primeiro, excluimos as proibidas
	my_grafo.updateIndex(); // depois, atualizamos os idexes das arestas no map
	obrigatorias = my_grafo.marcaObrigatorias(quantidadeObrigatorias); // determinanmos as obrigatorias
	nA= my_grafo.getQuantArestas();
	
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