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

// TODO : deve ser melhor que o Transgenético
// TODO : ver tempo e quantidade para instâncias grandes

#define maxSizePath 20 // TODO
#define maxVizinhos 10

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;
float yp, yq, xp, xq;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinalBuscaLocal ; // para medir o tempo

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

		A2 = new int[g->getQuantVertices()-1];
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


/*
#### Primeira fase ####
# A funcao phase1GM retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
list <pair<int*, pair<float, float> > > phase1GM(Grafo *g){
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
	retorna apenas solucoes que podem ser utilizadas no proximo passo do PR. max = maxSolucoes
	Esta funcao ja atualiza o vetor solucoes
*/
list <pair<int*,  pair<float, float> > > vizinhos(Grafo *g, pair<int*, pair<float, float> > sol, float cateto_x, float cateto_y, list <pair<int*, pair<float, float> > > &solucoes, int quantSols, int prob){
	list <pair<int*,  pair<float, float> > > retorno; /*possiveis candidatos*/
	for (int a = 0; a<g->getQuantVertices()-1 && retorno.size()<quantSols ; a++){ // n-1 (aresta que sairá)
		int dof = (rand()%101);
		if (g->getStatus(sol.first[a])==0 && dof<=20){ // so sai se for opcional. Se for obrigatoria, nao sai
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
		 	for (int i = 0; i <g->getQuantArestas() && retorno.size()<quantSols; i++){
		 		int odfff=(rand()%101);
		 		if (i!=idArestaSai && odfff<=20){
		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
		 				int newPeso1 = peso1base+g->getArestas(i)->getPeso1();
		 				int newPeso2 = peso2base+g->getArestas(i)->getPeso2();
		 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
		 					if (newPeso1<cateto_x && newPeso2<cateto_y){ // garante que o vizinho esteja dentro do triângulo retângulo 
		 						bool isdominada = false;
		 						for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end() && isdominada == false; it2++){
		 							if ((*it2).second.first<=newPeso1 && (*it2).second.second<=newPeso2 && ( (*it2).second.first<newPeso1 || (*it2).second.second<newPeso2)){
		 								isdominada = true;
		 							} else if ((*it2).second.first==newPeso1 && (*it2).second.second==newPeso2){
		 								isdominada = true;
		 							}
		 						}	
		 						vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
		 						for (list<pair<int*, pair<float, float> > >::iterator it2=retorno.begin(); it2!=retorno.end() && isdominada == false; it2++){
		 							if ((*it2).second.first<=newPeso1 && (*it2).second.second<=newPeso2 && ( (*it2).second.first<newPeso1 || (*it2).second.second<newPeso2)){
		 								isdominada = true;
		 							}else if ((*it2).second.first==newPeso1 && (*it2).second.second==newPeso2){
		 								isdominada = true;
		 							}
		 							if (newPeso1<=(*it2).second.first && newPeso2<=(*it2).second.second && ( newPeso1<(*it2).second.first || newPeso2<(*it2).second.second)){
		 								dominadas.push_back(it2); //se o vizinho domina algum outro vizinho
									}
		 						}
		 						if (isdominada==false){
			 						int *base = new int[g->getQuantVertices()-1];
				 					for (int are = 0; are <g->getQuantVertices()-1; are++){
				 						if (are == a) base[are] = i;
				 						else base[are] = sol.first[are];
				 					} 
				 					for (int iii=0; iii<dominadas.size(); iii++){
				 						delete[] (*dominadas[iii]).first;
					 					retorno.erase(dominadas[iii]); // retira as possiveis solucoes dominadas pela novas
					
				 					}
				 					retorno.push_back(make_pair(base, make_pair(newPeso1,newPeso2)));
				 					if (retorno.size()>=quantSols) break;
				 				}	
		 					}
		 				}
		 			}
		 		} 
		 	}
		 }
	}
	return retorno;
}

/*Quantidade de arestas que existem em xi (origem), mas que nao existem em xt(target). 
Ou seja, a quantidade de arestas que restam à xi para chegar até xt*/
int distance(Grafo *g, pair<int*, pair<float, float> > s1, pair<int*, pair<float, float> > s2){
	// int cont = 0;
	// bool tem = false;
	// if (xi.second.first == xt.second.first && xi.second.second == xt.second.second) return 0;
	// for (int i=0; i<g->getQuantVertices()-1; i++){ //arestas em xi
	// 	tem = false;
	// 	for (int t=0; t<g->getQuantVertices()-1; t++){ // arestas em xt
	// 		if (xt.first[t] == xi.first[i]) {
	// 			tem = true; break;
	// 		}
	// 	}
	// 	if (tem==false) cont++;
	// }
	// return cont;
	return sqrt((s1.second.first - s2.second.first)*(s1.second.first - s2.second.first) + (s1.second.second - s2.second.second)*(s1.second.second - s2.second.second));
}

void path_relinking(Grafo *g, pair<int*, pair<float, float> > x_startaux, pair<int*, pair<float, float> > x_target, list <pair<int*, pair<float, float> > > &solucoes, float cateto_x, float cateto_y, float escalarX, float escalarY, int maxSolPorTriangulo, int prob){
	pair<int*, pair<float, float> > x_start = make_pair(new int[g->getQuantVertices()-1], make_pair(x_startaux.second.first, x_startaux.second.second));
	for (int i=0; i<g->getQuantVertices()-1; i++) x_start.first[i] = x_startaux.first[i];
	float delta;
	int cont = 0;
	delta = distance(g, x_start, x_target);
	do {
		//delta = distance(g, x_start, x_target); 
		if (delta>=1 && cont<maxSizePath && solucoes.size()<maxSolPorTriangulo){
			list <pair<int*,  pair<float, float> > > viz = vizinhos(g, x_start,cateto_x, cateto_y,solucoes,maxSolPorTriangulo, prob); // somente os vizinhos dentro do triangulo formado pelos catetos x,y
			//float min = INT_MAX;
			//int min_delta = delta;
			//pair<int*, pair<float, float> > prox_start;
			// cout<<"\t\t viz.size() = "<<viz.size()<<endl;
			// cout<<"\t\t solucoes.size() antes = "<<solucoes.size()<<endl;
			vector <pair <int , pair<int*, pair<float, float> > > > prox_start;
			for (list<pair<int*, pair<float, float> > >::iterator viz_it=viz.begin(); viz_it!=viz.end(); viz_it++){ // coloca os vizinhos em retorno
				bool ha = false;
				float novoDelta = distance(g, *viz_it, x_target);
				if (novoDelta<=delta-1){
					// float custo = (*viz_it).second.first*escalarX + (*viz_it).second.second*escalarY;
					// if (custo<min){
					// 	min = custo;
					// 	prox_start = (*viz_it);
					// 	min_delta= novoDelta;
					// }
					prox_start.push_back(make_pair(novoDelta, (*viz_it)));
				}
				vector< list<pair<int*, pair<float, float> > >::iterator > dominadas;
				//saber se está na zona de esclusao; se nao estiver, a insere
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
					// 	::delete[] (*dominadas[iii]).first;
					 	solucoes.erase(dominadas[iii]); // retira as possiveis solucoes dominadas pela novas
					 }
					solucoes.push_back((*viz_it));
				} //else if ((prox_start.second.first != (*viz_it).second.first || prox_start.second.second != (*viz_it).second.second) && (x_start.second.first != (*viz_it).second.first || x_start.second.second != (*viz_it).second.second)) {
					//delete[] (*viz_it).first;
				//}
				//}
			}
			//cout<<"\t\t solucoes.size() antes = "<<solucoes.size()<<endl;
			if (prox_start.size()>0){
				int inx_random = rand()%prox_start.size();
				x_start = prox_start[inx_random].second;
				delta = prox_start[inx_random].first;
				// if (min_delta==delta) delta--;
				// else delta = min_delta;
				cont++;
			} else break;
		} else if (cont>=maxSizePath || solucoes.size()>=maxSolPorTriangulo){ //|| solucoes.size()>=maxSolPorTriangulo) {
			break;
		}
	}while(delta>1);
	cout<<"\t\tcont = "<<cont<<endl;
}

bool compare(pair<int*, pair<float, float> > p1, pair<int*, pair<float, float> > p2){
 	float escl1 = p1.second.first*(yp-yq)+p1.second.second*(xq-xp);
 	float escl2 = p2.second.first*(yp-yq)+p2.second.second*(xq-xp);
	return (escl1<escl2);
}

// /* 
// Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
// Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
// Assim, teremos separadas as solucoes suportadas e nao suportadas
// */
list < pair<int*, pair<float, float> > >  phase2KB(Grafo *g, list< pair<int*, pair<float, float> > > extremas){
	list< pair<int*, pair<float, float> > > noSoportadas; // resultado a ser retornado
	list< pair<int*, pair<float, float> > >::iterator it = extremas.begin(); 
	int contador = 0;
	int size = extremas.size();
	float areaTotal = 0;
	while (contador<size-1){
		pair<int*, pair<float, float> > ponto_p = *(it);
		pair<int*, pair<float, float> > ponto_q = *(++it);
		int *p = ponto_p.first; 
		int *q = ponto_q.first;
		
		xp = ponto_p.second.first;
		yp = ponto_p.second.second;
		xq = ponto_q.second.first;
		yq = ponto_q.second.second;

		float area = (((xq-xp)*(yp-yq))/2.0);
		areaTotal+=area;
		contador++;
	}
	contador = 0;
	it = extremas.begin(); 
	float toalprob = 0;
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

		float area = (((xq-xp)*(yp-yq))/2.0);
		int prob = (area*100)/areaTotal;
		int quantSols = 2000*prob/100;
		//cout<<"sols triângulo "<<contador<<" : "<<quantSols<<endl;
		if (quantSols>0){
			path_relinking(g, ponto_p, ponto_q, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp), quantSols,prob);
			//cout<<"\t\t noSuportadasPQ.size() = "<<noSuportadasPQ.size()<<endl;;
			path_relinking(g, ponto_q, ponto_p, noSuportadasPQ, xq, yp, (yp-yq), (xq-xp), quantSols,prob);
			noSoportadas.splice(noSoportadas.end(), noSuportadasPQ);
		}
		cout<<"noSuportadas.size() = "<<noSoportadas.size()<<endl;

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

	 list <pair<int*, pair<float, float> > > arvores = phase1GM(&my_grafo);
	
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