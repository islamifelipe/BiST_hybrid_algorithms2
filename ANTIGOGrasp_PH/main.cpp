/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements a hybrid GRASP algorithm with forward relinking (?) Path Relinking
# forward relinking: path-relinking is applied using the worst among xs and xt as the initial solution and the other as the target solution; (Resende e Ribeiro 2005)
#=======================================================================
*/

/*
	Reduz o grafo (SS 2008)
	calcula os vetores de escalarizacao
	Grasp{ (Baseado em parte no trabalho de Arroyo e Vianna)
		Constroi solucao 
		Busca local (PLS, baseado no algoritmo proposto por Drugan e Thierens (2012))
		PH truncado (Resende e Ribeiro (2005) e Laguna and Marti (1999))
	}
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
#include "Arquivo.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;

#define maxSearch 6
#define maxPR 7 // 7 // 10 é um bom valor (a priori)
#define numVetores 400
#define maxVizinhos 6 // 6

map <int, Aresta *> arestas;
Aresta ** arestasPtr;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinalBuscaLocal ; // para medir o tempo
Arquivo global_arc;
// TODO: tornar o grafo global

bool isEgalObjetive(float t1_peso1, float t1_peso2, float t2_peso1, float t2_peso2){
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);
}



// void borderSearch(Grafo *g, list<pair<int*, pair<float, float> > > &resul, pair<int*, pair<float, float> > &sl, pair<int*, pair<float, float> > &sll){ 
// 	int *s1 = sl.first;
// 	int *s2 = sll.first;
// 	int * A2;
// 	stack<pair<int*,int*> >  pilha;
// 	stack<pair<float, float> > pilhaX;
// 	stack<pair<float, float> > pilhaY;
// 	stack<pair<int,list<pair<int*, pair<float, float> > >::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	

// 	pilha.push(make_pair(s1, s2));
// 	pilhaX.push(make_pair(sl.second.first, sll.second.first)); //x's da primeira e sefunda solucao
// 	pilhaY.push(make_pair(sl.second.second, sll.second.second)); //y's da primeira e segunda solucao
// 	pilhaIt.push(make_pair(2, resul.begin()));


// 	while (pilha.size()!=0){
// 		pair<int*,int*> sols = pilha.top();
// 		pilha.pop();
// 		s1 = sols.first;
// 		s2 = sols.second;
// 		pair<int,list<pair<int*, pair<float, float> > >::iterator>  it = pilhaIt.top();
// 		pilhaIt.pop();

// 		pair<float, float> xs = pilhaX.top();
// 		pair<float, float> ys =  pilhaY.top();
// 		float xl = xs.first;
// 		float yl = ys.first;
// 		float xll = xs.second;
// 		float yll = ys.second;

// 		pilhaX.pop();
// 		pilhaY.pop();

// 		//A2 = new int[g->getQuantArestas()];
// 		A2 = new int[g->getQuantVertices()-1];
// 		float cont; // nao utilisazado nesse caso
// 		mergesort(xl, yl, xll, yll, arestasPtr, g->getQuantArestas(),3);
// 		float x, y;
// 		kruskal(g, arestasPtr,A2,x, y);
// 		if( !( (isEgalObjetive(x, y, xl, yl)) || (isEgalObjetive(x, y, xll, yll)) ) ){
// 			if (it.first == 1){ // antes
// 				resul.insert(it.second, make_pair(A2, make_pair(x,y)));//A2); 
// 				it.second--;// it agora aponta para o item  A2
// 			} else if (it.first == 2) { // depois
// 				it.second++;
// 				resul.insert(it.second, make_pair(A2, make_pair(x,y)));
// 				it.second--;// it agora aponta para o item  A2
// 			}

// 			pilha.push(make_pair(A2, s2)); // L''
// 			pilhaIt.push(make_pair(2,it.second)); 
// 			pilhaX.push(make_pair(x, xll));
// 			pilhaY.push(make_pair(y, yll));

// 			pilha.push(make_pair(s1, A2));  // L'
// 			pilhaIt.push(make_pair(1,it.second)); 
// 			pilhaX.push(make_pair(xl, x));
// 			pilhaY.push(make_pair(yl, y));
			
// 		}
	
// 	}
// }


// list <pair<int*, pair<float, float> > > buscaDicotomica(Grafo *g){
// 	list< pair<int*, pair<float, float> > > result;
// 	int *s1 = new int[g->getQuantVertices()-1];
// 	float xr, yr; // nao utilisazado nesse caso
// 	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),1);
// 	kruskal(g, arestasPtr,s1,xr, yr); // arvore para o primeiro objetivo
// 	 pair<int*, pair<float, float> > ps1 = make_pair(s1, make_pair(xr,yr));
// 	result.push_back(ps1);
// 	int* s2 = new int[g->getQuantVertices()-1];
// 	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),2);
// 	float xr2, yr2;
// 	kruskal(g, arestasPtr, s2,xr2, yr2); // arvore para o segundo objetivo
// 	pair<int*, pair<float, float> > ps2 = make_pair(s2, make_pair(xr2,yr2));
	
// 	if (isEgalObjetive(xr, yr, xr2, yr2)==false){
// 		borderSearch(g, result, ps1, ps2);
// 		result.push_back(ps2);
// 	}
// 	return result;
//  }

pair<int*, pair<float, float> > clone(Grafo *g, pair<int*, pair<float, float> > &s){
	pair<int*, pair<float, float> > ret = make_pair(new int[g->getQuantVertices()-1], make_pair(0,0));
	for (int i=0;i<g->getQuantVertices()-1; i++) ret.first[i] = s.first[i];
	ret.second.first = s.second.first;
	ret.second.second = s.second.second;
	return ret;
}

//retorna “verdadeiro” caso sl esteja em uma regiao menos populosa que s com relacao ao arquivo local_arc.
bool m_grid(pair<int*, pair<float, float> > sl, pair<int*, pair<float, float> > s){
	return global_arc.getPositionCount(sl)<global_arc.getPositionCount(s);
}


/*retorna a distância euclidiana entre dois pontos*/
float distancia(pair<float, float> s1, pair<float, float> s2){
	return sqrt((s1.first - s2.first)*(s1.first - s2.first) + (s1.second - s2.second)*(s1.second - s2.second));
}

/* 	
	uma arvore s' é vizinha de s sse elas diferem em somente uma aresta
	retorna um subconjunto randômico dos vizinhos de "s" que nao sao dominados por "s"
*/
list <pair<int*,  pair<float, float> > > vizinhos(Grafo *g, pair<int*, pair<float, float> > sol){
	list <pair<int*,  pair<float, float> > > retorno;
	for (int ccc = 0; ccc<maxVizinhos; ccc++){
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
				
				pair<int*,  pair<float, float> > fknrjgnj = make_pair(base, make_pair(newPeso1,newPeso2));
				global_arc.adicionarSol(clone(g,fknrjgnj));
				retorno.push_back(fknrjgnj); // adiciona no conjunto de retorno apenas se estiver em As	
			}
		} // else fora
	}

	return retorno;
}

void path_relinking(Grafo *g, pair<int*, pair<float, float> > x_startaux, pair<int*, pair<float, float> > x_target, float lambda){
	float distanciaOriginal;
	int contMax = 0;
	pair<int*, pair<float, float> > startaux = clone(g, x_startaux);

	do{
		distanciaOriginal = distancia(startaux.second, x_target.second);
		if (distanciaOriginal>0){
			list <pair<int*,  pair<float, float> > > viz = vizinhos(g, startaux);
			vector <pair<int*,  pair<float, float> > > vizi2;
			for (list <pair<int*,  pair<float, float> > >::iterator in=viz.begin(); in!=viz.end(); in++){
				if (distancia((*in).second, x_target.second)<distanciaOriginal) vizi2.push_back((*in));
			}

			if (vizi2.size()>0){
				pair<int*, pair<float, float> > proximooo = vizi2[rand()%vizi2.size()]; // sortea o proximo a ser verificado
				contMax++;
				for (int i=0; i<g->getQuantVertices(); i++) startaux.first[i] = proximooo.first[i];
				startaux.second.first = proximooo.second.first;
				startaux.second.second = proximooo.second.second;
			} else {
				contMax++;
			}
		}
	}while (distanciaOriginal > 0 && contMax<maxPR);
	delete[] startaux.first;
}

//edita sol com uma solucao que minimiza o custo escalarizado
void localSearch(Grafo *g, pair<int*, pair<float, float> >  &sol, float lambda){
	for (int i=0; i<maxSearch; i++){
		list <pair<int*,  pair<float, float> > > viz = vizinhos(g,sol);
		list<pair<int*, pair<float, float> > >::iterator it = viz.begin();
		float min = (*it).second.first*lambda + (*it).second.second*(1.0-lambda);
		bool modificou = false;
		for (list<pair<int*, pair<float, float> > >::iterator it3=viz.begin(); it3!=viz.end(); it3++){
			float custo = (*it3).second.first*lambda + (*it3).second.second*(1.0-lambda);
			//arc.adicionarSol(clone((*it3)));
			if (custo<min || m_grid((*it3), (*it))==true){
				min = custo;
				it = it3;
				modificou = true;
			}
		}
		if (modificou==true){
			//cout<<"Melhorou"<<endl;
			for (int kgo = 0; kgo<g->getQuantVertices()-1; kgo++){
				sol.first[kgo] = (*it).first[kgo];
			}
			sol.second.first = (*it).second.first;
			sol.second.second = (*it).second.second;
		}
	}
}

// void geraVetoresEscalarizacao () {

//     float v = 0.0;
//     float fator = 1.0;
//     for (int i = 0; i < numVetores; i++) {
//         float lambda = v/(numVetores-1.0);
//         vetoresEscalarizacao.push_back(lambda);
//         //lambda [i][1] = 1.0 - lambda [i][0];
//         v += fator;
//     }
// }

void grasp(Grafo *g, float alfa){
	
	//geraVetoresEscalarizacao ();
	//cout<<"Quantidade de vetores calculados = "<<vetoresEscalarizacao.size()<<endl;
	float v = 0.0;
	float fator = 1.0;
	for (int itera = 0; itera<numVetores; itera++){
		float lambda = v/(numVetores-1.0);
		v += fator;
		cout<<"Itaracao = "<<itera<<endl;
		int *nova = new int[g->getQuantVertices()-1];
		//float lambda = vetoresEscalarizacao[itera];
		mergesort(0, lambda, 1.0-lambda, 0, arestasPtr, g->getQuantArestas(),3);
		//pair<float, Aresta**> presort = preSorting[itera];
		float peso1=0, peso2=0;
		int cont = 0;
		Conjunto conc(g->getQuantVertices());
		vector<Aresta *> amostral;
		for (int i=0; i<g->getQuantArestas(); i++){
			Aresta *a = arestasPtr[i];
			if (g->getStatus(a->getId())==1){ // obrigatoria
				nova[cont++] = a->getId();
				conc.union1(a->getOrigem(), a->getDestino());
				peso1+= a->getPeso1();
				peso2+= a->getPeso2();
			} else{
				amostral.push_back(a);
			}
		}
		int rlc = alfa*amostral.size();
		if (rlc<1) rlc = 1;
		//cout<<"rlc = "<<rlc<<"  amostral.size() = "<<amostral.size()<<endl;
		while (cont<g->getQuantVertices()-1 && rlc>0){
			int index = rand()%(rlc); // entre 0 e rlc-1
			Aresta *a = amostral[index];
			if (conc.compare(a->getOrigem(),a->getDestino())==false){
				nova[cont++] = a->getId();
				conc.union1(a->getOrigem(),a->getDestino());
				peso1+= a->getPeso1();
				peso2+= a->getPeso2();
			
			} 
			amostral.erase(amostral.begin()+index);
			rlc--;
		}
		if (cont<g->getQuantVertices()-1){ // se por acaso, o laço acima terminar sem uma arvore estar formada, entao adcionamos as menores arestas
			for (int i=0; i<amostral.size() && cont<g->getQuantVertices()-1; i++){
				Aresta *a = amostral[i];
				if (conc.compare(a->getOrigem(),a->getDestino())==false){
					nova[cont++] = a->getId();
					conc.union1(a->getOrigem(),a->getDestino());
					peso1+= a->getPeso1();
					peso2+= a->getPeso2();
				} 
			}
		}
		if (cont==g->getQuantVertices()-1){
			pair<int*, pair<float, float> > sol1 = make_pair(nova, make_pair(peso1,peso2));
			global_arc.adicionarSol(clone(g, sol1));
			cout<<"global_arc.size() BL antes = "<<global_arc.getSize()<<endl;
			
			localSearch(g, sol1,lambda);
			cout<<"global_arc.size() BL depois = "<<global_arc.getSize()<<endl;
			// TODO : CUIDADO: SE O IT FOR DELETADO DO ARQUIVO, O PR PODE CAUSAR SEGMENTATION FAUT; TEM QUE FAZER UMA COPIA 
			list <pair<int*, pair<float, float> > > arvores = global_arc.getSolucoes();
			int indx = rand()%arvores.size();
			list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); 
			for (int cont=0;it!=arvores.end() && cont<indx; it++, cont++);
			cout<<"global_arc.size() PR antes = "<<global_arc.getSize()<<endl;
			path_relinking(g, *it, sol1,lambda);
			cout<<"global_arc.size() PR depois = "<<global_arc.getSize()<<endl;
			
		}
		//delete[] preSorting[itera].second;
	}
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

	times(&tempsInit); // o tempo inclui o calculo dos vetores

 	//list <pair<int*, pair<float, float> > > arvores;//=buscaDicotomica(&my_grafo);
	grasp(&my_grafo, 0.03);
	//cout<<"Fim da primeira fase ... "<<endl;
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	//cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<" segundos"<<endl;//"Tempo do usuario por segundo : "
   	
	
    int i = 1, cont=0;
 	list <pair<int*, pair<float, float> > > arvores = global_arc.getSolucoes();
 	float peso1max = -1, peso2max = -1;
 	float cont1=0, cont2=0;
 	cout<<"Quantidade de solucoes = "<<arvores.size()<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		pair<int*, pair<float, float> > arvr;
		arvr = (*it);
		//cout<<"Arvore "<<i<<endl;
		cont1=0; cont2=0;
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
    	if (cont1!=arvr.second.first || cont2!=arvr.second.second) cout<<"ERRRRRRROOOOOO   :    ";
    	cout<<arvr.second.first<<" "<<arvr.second.second<<endl;
    	if (arvr.second.first>peso1max) peso1max = arvr.second.first;
    	if (arvr.second.second>peso2max) peso2max = arvr.second.second ;
    	i++;

	}
	cout<<"MAX = "<<peso1max<<" "<<peso2max<<endl;
	
	return 0;
}