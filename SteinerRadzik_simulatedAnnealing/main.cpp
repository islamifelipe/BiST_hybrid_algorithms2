/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements a hybrid Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The second phase uses a Multi-objective Simulated annealing (MOSA, Bandyopadhyay et al (2008))
#=======================================================================
*/

/*
	Falta:
		--> Ajusar Parâmetros
		--> Lista Tabu
		--> cadinalidade des suportadas e nao suportadas
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <vector>
#include <cmath>       /* exp */
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

#define interacoes 50
#define alfa 0.80
#define temp_init 90
#define temp_final 2

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;
float yp, yq, xp, xq;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinalBuscaLocal ; // para medir o tempo
list < pair<int*, pair<float, float> > > MSTs;

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

/*Seleciona uma aresta fora da arvore, a insere formando ciclo e remove alguma do ciclo. 
As duas arvores diferirao em apenas uma aresta, logo, serao vizinhas*/
 pair<int*,  pair<float, float> > vizinhoRadom(Grafo *g,  pair<int*,  pair<float, float> > sol){
 	 pair<int*,  pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
 	 vector<int> opcionais;
 	 for (int i=0; i<g->getQuantVertices()-1; i++){
 	 	if (g->getStatus(sol.first[i])==0){
 	 		opcionais.push_back(sol.first[i]);
 	 	}
 	 }
 	 if (opcionais.size()==0){
 	 	delete[] ret.first;
	  	return sol;
 	 }
 	 int pos = rand()%(opcionais.size());
 	 int idSaiRadom = opcionais[pos];
 	 Conjunto con(g->getQuantVertices());
 	 for (int i=0; i<g->getQuantVertices()-1; i++){
 	 	if (sol.first[i]!=idSaiRadom){
 	 		con.union1(g->getArestas(sol.first[i])->getDestino(), g->getArestas(sol.first[i])->getOrigem());
 	 		ret.first[i] = sol.first[i];
 	 	} else {
 	 		pos = i;
 	 	}
 	 }
	 Aresta *a = g->getArestas(idSaiRadom);
 	 ret.second.first = sol.second.first-a->getPeso1();
 	 ret.second.second = sol.second.second-a->getPeso2();
 	 vector<int> amostral;
 	 Vertice *o = g->getVertice(a->getOrigem());
 	 Vertice *d = g->getVertice(a->getDestino());
 	 for (int i=0; i<o->getGrau(); i++){ // pior caso: m-1 O(m)
 	 	if (o->getAresta(i)->getId()!=idSaiRadom){
 	 		if (con.compare(o->getAresta(i)->getOrigem(), o->getAresta(i)->getDestino())==false){
 	 			amostral.push_back(o->getAresta(i)->getId());
 	 		}
 	 	}
 	 }
 	 for (int i=0; i<d->getGrau(); i++){ // pior caso: m-1 O(m)
 	 	if (d->getAresta(i)->getId()!=idSaiRadom){
 	 		if (con.compare(d->getAresta(i)->getOrigem(), d->getAresta(i)->getDestino())==false){
 	 			amostral.push_back(d->getAresta(i)->getId());
 	 		}
 	 	}
 	 }
 	if (amostral.size()>0){
	 	 int r = rand()%(amostral.size());
	 	 int idNewAresta = amostral[r];
	 	 ret.first[pos] = idNewAresta;
	 	 ret.second.first += g->getArestas(idNewAresta)->getPeso1();
	 	 ret.second.second += g->getArestas(idNewAresta)->getPeso2();
	 	 return ret;
	 } else {
	 	delete[] ret.first;
	  	return sol;
	 }
 }
bool a_domina_b(pair<float, float> a, pair<float, float> b){
	return (a.first <= b.first && a.second <= b.second && (a.first < b.first || a.second < b.second));
}
 void mosa(Grafo *g, pair<int*, pair<float, float> > sol1,list <pair<int*, pair<float, float> > > &solucoes){
 	float temp = temp_init;
 	while (temp>temp_final){
 		for (int i=0; i<interacoes; i++){
 			pair<int*,  pair<float, float> > viz = vizinhoRadom(g, sol1);
 			bool igual = false;
 			bool dominada = false;
 			int energia_sol = 0;
 			int energia_nova = 0;
 			for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
				if (viz.second.first ==(*it2).second.first && viz.second.second == (*it2).second.second){
					igual = true;
					break;
				} else {
					if (a_domina_b((*it2).second, viz.second) == true){
						dominada = true;
						energia_nova++;
					}
					if (a_domina_b((*it2).second, sol1.second) == true){
						energia_sol++;
					}
				}
			}
			if (igual==false){
				if (dominada==true || a_domina_b(sol1.second, viz.second)==true){
					//complementamos o calculo da energia com as solucoes em MST
					// for (list<pair<int*, pair<float, float> > >::iterator it2=MSTs.begin();it2!=MSTs.end(); it2++){
					// 	if (a_domina_b((*it2).second, viz.second) == true){
					// 		energia_nova++;
					// 	} 
					// 	if (a_domina_b((*it2).second, sol1.second) == true){
					// 		energia_sol++;
					// 	}
					// }
					float delta = 0;
					if (solucoes.size()>0) delta = (energia_sol - energia_nova)/(solucoes.size());
					double probabilidade = exp ((-1)*(delta)/temp);
					//cout<<"oekfokeofkeofk"<<endl;
					if (((rand() % 101)/100)<probabilidade){
						solucoes.push_back(viz);
						sol1 = viz;
					} else {
						delete[] viz.first;
					}
				} else {
					solucoes.push_back(viz);
					sol1 = viz;
				}
			}
 		}
 		temp = alfa*temp;
 	}
 }

// /* 	
// 	uma arvore s' é vizinha de s sse elas diferem em somente uma aresta
// 	retorna os vizinhos de "s" que nao sao dominados por "s"
// */
// list <pair<int*,  pair<float, float> > > vizinhos(Grafo *g, pair<int*, pair<float, float> > sol){
// 	list <pair<int*,  pair<float, float> > > retorno;
// 	for (int a = 0; a<g->getQuantVertices()-1; a++){ // n-1 (aresta que sairá)
// 		if (g->getStatus(sol.first[a])==0){ // so sai se for opcional. Se for obrigatoria, nao sai
// 			int idArestaSai = sol.first[a];
// 			Conjunto conjunto(g->getQuantVertices());
// 			for (int i = 0; i <g->getQuantVertices()-1; i++){
// 				if (i!=a){
// 					conjunto.union1(g->getArestas(sol.first[i])->getOrigem(), g->getArestas(sol.first[i])->getDestino());
					
// 				}
// 			}
// 			int peso1base = sol.second.first - g->getArestas(sol.first[a])->getPeso1();
// 			int peso2base = sol.second.second - g->getArestas(sol.first[a])->getPeso2();
// 			vector<int> arestasPossiveis;
// 		 	for (int i = 0; i <g->getQuantArestas(); i++){
// 		 		if (i!=idArestaSai){
// 		 			int www = i; // deve ser menor
// 		 			int kkk = idArestaSai;
// 		 			if (www>kkk){
// 		 				www = idArestaSai;
// 		 				kkk = i;
// 		 			}
// 		 			if (conjunto.compare(g->getArestas(i)->getOrigem(), g->getArestas(i)->getDestino())==false){
// 		 				int newPeso1 = peso1base+g->getArestas(i)->getPeso1();
// 		 				int newPeso2 = peso2base+g->getArestas(i)->getPeso2();
// 		 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
// 		 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
// 		 					bool entra = true;
// 		 					for (int oooo=0; oooo<lista_tabu[www][kkk].size(); oooo++){
// 		 						if ((lista_tabu[www][kkk][oooo].first == newPeso1 && lista_tabu[www][kkk][oooo].second == newPeso2)) {
// 		 							entra = false;
// 		 						}
// 		 					}
		 					
// 		 					if (entra==true){
// 			 					int *base = new int[g->getQuantVertices()-1];
// 			 					for (int are = 0; are <g->getQuantVertices()-1; are++){
// 			 						if (are == a) base[are] = i;
// 			 						else base[are] = sol.first[are];
// 			 					} 
// 			 					lista_tabu[www][kkk].push_back(make_pair(sol.second.first, sol.second.second));
// 			 					lista_tabu[www][kkk].push_back(make_pair(newPeso1, newPeso2));
// 			 					retorno.push_back(make_pair(base, make_pair(newPeso1,newPeso2)));
// 		 					}	
// 		 				}
// 		 			}
			 		
// 		 		} 
// 		 	}
// 		 }
// 	}
// 	return retorno;
// }


// /*
// 	A lista tabu list_tabu é uma matriz triangular superior do tamanho da quantidade de arestas.
// 	se list_tabu[i][j] = vector de pontos gerados pela troca entre i e j; quer dizer que a aresta de id i ainda nao foi trocada com j
	
// */
// void localSearch(Grafo *g, pair<int*, pair<float, float> > sol1,list <pair<int*, pair<float, float> > > &solucoes){
// 	queue <pair<int*, pair<float, float> > > fila;
// 	for (int iii=0; iii<g->getQuantArestas(); iii++){
// 		for (int jjj=iii+1; jjj<g->getQuantArestas(); jjj++){
// 			lista_tabu[iii][jjj].clear();
// 		}
// 	}
// 	fila.push(sol1); 
// 	int contIteracoes = 0;
// 	while (fila.size()>0 && contIteracoes<maxIteracoes){
// 		pair<int*, pair<float, float> > sol = fila.front();
// 		fila.pop();
// 		list <pair<int*,  pair<float, float> > > viz = vizinhos(g, sol);
// 		// é garantido que os vizinhos de "itlocal" nao sao dominados por "itlocal"
// 		for (list<pair<int*, pair<float, float> > >::iterator viz_it=viz.begin(); viz_it!=viz.end(); viz_it++){ // coloca os vizinhos em retorno
// 			bool ha = false;
// 			for (list<pair<int*, pair<float, float> > >::iterator it2=solucoes.begin(); it2!=solucoes.end(); it2++){
// 				if (((*viz_it).second.first ==(*it2).second.first && (*viz_it).second.second == (*it2).second.second)){
// 					ha = true;
// 					break;
// 				} else if (((*it2).second.first<=(*viz_it).second.first && (*it2).second.second<=(*viz_it).second.second && ((*it2).second.first<(*viz_it).second.first || (*it2).second.second<(*viz_it).second.second))){
// 					ha = true;
// 					break;
// 				} 
// 			}
// 			if (ha==false){
// 				fila.push((*viz_it));
// 				solucoes.push_back((*viz_it));
// 			}
// 		}
// 		contIteracoes++;
// 	}
// }


// /* 
// # retorna verdadeiro se o ponto kbest está dentro da regiao viável
// # se verdeiro, atualiza a regiao viável
// */	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y){
	bool retorno = false; // por default, o ponto nao está na regiao viavel 
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o k_best_x deve ser menor que o ponto_x   E   o k_best_y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o kbest nao está na regiao viavel*/ 
		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
			pair<float, float> ponto1 = make_pair(x, ponto_y);
			pair<float, float> ponto2 = make_pair(ponto_x, y);
			regiaoViavel.insert(it, ponto1);
			regiaoViavel.insert(it, ponto2);
			regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
			retorno = true;
			break; // se o ponto está regiao viavel, nao há mais rezao para percorrer-la 
		}
	}
	return retorno;
}

pair<float, float> getMaiorDistante(float a, float b, float c, list< pair<float, float> > regiaoViavel){
	pair<float, float>  pontoR = (*regiaoViavel.begin());
	float max = ((float)abs(a*pontoR.first + b*pontoR.second + c))/((float) sqrt(a*a + b*b));
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it);
		float distancia = abs(a*ponto.first + b*ponto.second + c)/sqrt(a*a + b*b); // distância de ponto a r
 		if (maiorQuefloat(distancia,max)){
 			max = distancia;
 			pontoR = ponto;
 		}
	}
	return pontoR;
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
	while (contador<size-1){
		pair<int*, pair<float, float> > pp = *(it);
		pair<int*, pair<float, float> > pq = *(++it);
		int *p = pp.first; 
		int *q = pq.first;

		list< pair<float, float> > regiaoViavel; // lista de pontos que delimitam a regiao viável.
	
		xp = pp.second.first;
		yp = pp.second.second;
		xq = pq.second.first;
		yq = pq.second.second;
		if (abs(yp-yq)==1 || abs(xq-xp)==1) { contador++;  continue;}
		if (contador == 0){
			mosa(g, pp,MSTs);
			mosa(g, pq,MSTs);
		} else {
			mosa(g, pq,MSTs);
		}
		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		
		// determina a reta p-q (hipotenusa)
		float a, b; // pra determinar a equacao da reta p-q na forma ax+b = y
		a = ((float)(yp-yq))/((float) (xp-xq)); // coeficiente angular da reta p-q
		b = yq - a*xq; // coeficiente linear de p-q
		pair<float, float> maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
		//Agora determinamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
		float bM = maisDistante.second - a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y
		int k = 0;
		MSTs.sort(compare);
		list< pair<int*, pair<float, float> > >::iterator kbest = MSTs.begin();
		for (; k<MSTs.size(); k++){
			pair<int*, pair<float, float> >  k_best = *kbest;
			float x = k_best.second.first;
			float y= k_best.second.second;
			if (isInViableRegion(g, regiaoViavel, x, y)){
				noSoportadas.push_back(k_best);
				maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
				kbest = MSTs.erase(kbest);
				mosa(g, k_best,MSTs);
				MSTs.sort(compare);
				//Agora atualizamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
				bM = (float)maisDistante.second - (float)a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y
			} else if ( maiorIgualQuefloat(y,(a*x+bM))) { //s on or past maximum cost line 
				break;
			} else kbest++;
		}
		contador++;
	}
	return noSoportadas;

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
 	times(&tempsInit);

	 list <pair<int*, pair<float, float> > > arvores = phase1GM(&my_grafo);
	
	cout<<"Fim da primeira fase ... "<<endl;
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	//cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);


	list <pair<int*, pair<float, float> > > noSuportadas = phase2KB(&my_grafo, arvores);
	

	cout<<"Fim da segunda fase ... "<<endl;

	times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	//cout<<user_time2<<endl;
	cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	cout<<"Total ..."<<endl;
	cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;

	
    int i = 1, cont=0;
 
    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		pair<int*, pair<float, float> > arvr;
		//pertubacao(&my_grafo,(*it),arvr);
		arvr = (*it);
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			int iddd = (arvr.first)[a];
				cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
    			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
    			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
    			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
    		
    	}
    	cout<<"("<<arvr.second.first<<", "<<arvr.second.second<<")\n"<<endl;
   //  	pair<int*, pair<float, float> > viz = vizinhoRadom(&my_grafo,arvr);
   //  	cout<<"Vizinho "<<i<<endl;
   //  	for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			// int iddd = (viz.first)[a];
			// 	cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
   //  			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
   //  			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
   //  			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
    		
   //  	}
   //  	cout<<"("<<viz.second.first<<", "<<viz.second.second<<")\n"<<endl;
    	
    	i++;
	}


	cout<<"Resultado \n NAO SUPORTADAS"<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			int iddd = ((*it).first)[a];
				cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
    			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
    			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
    			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
    		
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}
	
	return 0;
}