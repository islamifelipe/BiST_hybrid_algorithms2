/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements a hybrid Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The second phase uses ScatterSearch algoritm
#=======================================================================
*/


#include <iostream>
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
#include "Arquivo.h"
#include <climits>
using namespace std;

// #define maxSizePath 5
#define maxVizinhos 8 //40
#define maxSuportadas 50
#define maxSearch 10 //10 //9 5
#define maxSolInit 10 // 10
#define maxIteracoes 6 //15 
#define maxSubsets 6 // 20 
// #define referenceSetSize 18 //
// #define referenceSetSize1 15 // referenceSetSize1 < referenceSetSize 
#define limiteSols 55
#define maxPR 8 // 25
#define alpha 0.01

typedef struct { // pra calcular as solucoes suportadas
	pair<int*, pair<float, float> > s1; 
	pair<int*, pair<float, float> > s2; 
} ItemFila;

Aresta ** arestasPtr;
float yp, yq, xp, xq;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinalBuscaLocal ; // para medir o tempo
int arestasObrigatorias;
pair<int*, pair<float, float> > obrigatorias;
Grafo g;
Arquivo globalSolutions;

pair<int*, pair<float, float> > clone(pair<int*, pair<float, float> > &s){
	pair<int*, pair<float, float> > ret = make_pair(new int[g.getQuantVertices()-1], make_pair(0,0));
	for (int i=0;i<g.getQuantVertices()-1; i++) ret.first[i] = s.first[i];
	ret.second.first = s.second.first;
	ret.second.second = s.second.second;
	return ret;
}

/*Esta funcao é adaptada do algoritmo propsoto na dissertacao de Monteiro (2010)
O procedimento usa uma filha pra obter solucoes suportadas bem distribuidas na fronteira de pareto
O procedimento termina ou quando o limite de iteracoes é atingido ou quando acaba as solucoes suportadas*/
list <pair<int*, pair<float, float> > >  suportadas(){
	list <pair<int*, pair<float, float> > > setSolucoes;
	Aresta **arestasPtr = g.getAllArestasPtr();
	queue <ItemFila> fila;
	int *s1 = new int[g.getQuantVertices()-1]; // primeiro objetivo
	float x=0, y=0; // objetivos
	mergesort(0, 0, 0, 0, arestasPtr, g.getQuantArestas(),1);
	kruskal(&g, arestasPtr,s1,x, y); // arvore para o primeiro objetivo
	pair<int*, pair<float, float> > ps1 = make_pair(s1, make_pair(x,y));
	setSolucoes.push_back(ps1);
	int* s2 = new int[g.getQuantVertices()-1];
	mergesort(0, 0, 0, 0, arestasPtr, g.getQuantArestas(),2);
	x=0, y=0;
	kruskal(&g, arestasPtr, s2,x, y); // arvore para o segundo objetivo
	pair<int*, pair<float, float> > ps2 = make_pair(s2, make_pair(x,y));
	setSolucoes.push_back(ps2);
	fila.push((ItemFila){ps1, ps2});

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

		int *A2 = new int[g.getQuantVertices()-1];
		mergesort(xl, yl, xll, yll, arestasPtr, g.getQuantArestas(),3);
		x=0; y=0;
		kruskal(&g, arestasPtr,A2,x, y);

		if (x!=xl && x!=xll && y!=yl && y!=yll){
			pair<int*, pair<float, float> > novo = make_pair(A2, make_pair(x, y));
				
			if (cont<maxSuportadas){
				setSolucoes.push_back(novo);
				cont++;
			}
			fila.push((ItemFila){s1, novo});
			fila.push((ItemFila){novo, s2});
		}

	}
	return setSolucoes;
}


list <pair<int*,  pair<float, float> > > vizinhos2(pair<int*, pair<float, float> > sol, float cateto_x, float cateto_y, float escalarY, float escalarX, Arquivo &arc){
	list <pair<int*,  pair<float, float> > > retorno; /*possiveis candidatos*/
	int custo = sol.second.first*escalarY + sol.second.second*escalarX; // int mesmo
	for (int viz=0; viz<maxVizinhos && custo!=0; viz++){
		//float randdd = rand()%custo;
		int randdd = rand()%(g.getQuantVertices()-1);
		if (g.getStatus(sol.first[randdd])!=0) continue;
		int idEscolhidaSair=sol.first[randdd];//0;
		int soma = 0;
		Conjunto conjunto(g.getQuantVertices());
		for (int aa = 0; aa<g.getQuantVertices()-1; aa++){
			//int custoAresta = g.getArestas(sol.first[aa])->getPeso1()*escalarY + g.getArestas(sol.first[aa])->getPeso2()*escalarX;
			//if (randdd>=soma && randdd<custoAresta+soma){ // aresta foi escolhida
			//if (g.getStatus(sol.first[aa]))
			//	idEscolhidaSair = sol.first[aa];
			//} else { // aresta fica, ou seja, nao foi escolhida pra sair
			if (idEscolhidaSair!=sol.first[aa]){
				conjunto.union1(g.getArestas(sol.first[aa])->getOrigem(), g.getArestas(sol.first[aa])->getDestino());	
			}
			//soma+=custoAresta;
		}
		//if (g.getStatus(idEscolhidaSair)==0){
			
		int peso1base = sol.second.first  - g.getArestas(idEscolhidaSair)->getPeso1();
		int peso2base = sol.second.second - g.getArestas(idEscolhidaSair)->getPeso2();
		///cout<<"Sai aresta id="<<idEscolhidaSair<<" Peso1 = "<<g.getArestas(idEscolhidaSair)->getPeso1()<<" Peso2 = "<<g.getArestas(idEscolhidaSair)->getPeso2()<<endl;
		vector<int> arestasPossiveis;
		for (int i = 0; i <g.getQuantArestas(); i++){
	 		if (i!=idEscolhidaSair){
	 			if (conjunto.compare(g.getArestas(i)->getOrigem(), g.getArestas(i)->getDestino())==false){
	 				int newPeso1 = peso1base+g.getArestas(i)->getPeso1();
	 				int newPeso2 = peso2base+g.getArestas(i)->getPeso2();
	 				// so insere se o vizinho nao for dominado por sol. Nao veriica se sol é dominado...
	 				if ( (sol.second.first <= newPeso1 &&  sol.second.second <= newPeso2 && (sol.second.first < newPeso1 ||  sol.second.second < newPeso2))==false){
	 					//if (newPeso1<cateto_x && newPeso2<cateto_y){ // garante que o vizinho esteja dentro do triângulo retângulo 
	 					arestasPossiveis.push_back(i);
	 					//}
	 				}
	 			}
	 		}
	 	}
	 	if (arestasPossiveis.size()>0){
	 		int idEscolhidaEntra = arestasPossiveis[rand()%arestasPossiveis.size()];
	 		int *base = new int[g.getQuantVertices()-1];
	 		int *copia1 = new int[g.getQuantVertices()-1]; // poupa um laço no clone
			for (int are = 0; are <g.getQuantVertices()-1; are++){
			 	if (sol.first[are] == idEscolhidaSair) {
			 		base[are] = idEscolhidaEntra;
			 		copia1[are] = idEscolhidaEntra;
			 	}
			 	else {
			 		base[are] = sol.first[are];
			 		copia1[are] = sol.first[are];
			 	}
			} 
			pair<int*,  pair<float, float> > tree = make_pair(base, make_pair(peso1base+g.getArestas(idEscolhidaEntra)->getPeso1(),peso2base+g.getArestas(idEscolhidaEntra)->getPeso2()));
				
			if (tree.second.first<cateto_x && tree.second.second<cateto_y){ // garante que o vizinho esteja dentro do triângulo retângulo 
	 			pair<int*,  pair<float, float> > treecopia = make_pair(copia1, make_pair(peso1base+g.getArestas(idEscolhidaEntra)->getPeso1(),peso2base+g.getArestas(idEscolhidaEntra)->getPeso2()));
				arc.adicionarSol(treecopia); //foi garantido que o vizinho está dentro do triângulo retângulo
				retorno.push_back(tree);
			} else{
				globalSolutions.adicionarSol22(tree);
			}
	 	}
		//} //else fora
	}
	return retorno;
}


// float distance1(pair<int*, pair<float, float> > s1, pair<int*, pair<float, float> > s2){
	
// 	return sqrt((s1.second.first - s2.second.first)*(s1.second.first - s2.second.first) + (s1.second.second - s2.second.second)*(s1.second.second - s2.second.second));
// }

/*retorna a distância euclidiana entre dois pontos*/
float distancia(pair<float, float> s1, pair<float, float> s2){
	return sqrt((s1.first - s2.first)*(s1.first - s2.first) + (s1.second - s2.second)*(s1.second - s2.second));
}

// vector<pair <Aresta* , int > > restricted_list(float lambda1,float lambda2, Aresta** L, float num, Conjunto &conjunto){
// 	//Aresta ** L = presort;
// 	vector<pair <Aresta* , int > > ret;
// 	bool first = false;
// 	float w = -1;
// 	for (int i=0; i<g.getQuantArestas(); i++){
// 		if (L[i] != NULL){
// 			if (first==false) {
// 				w = L[i]->getPeso1()*(lambda1) + L[i]->getPeso2()*(lambda2);
// 				first = true;
// 			}
// 			float peso = L[i]->getPeso1()*(lambda1) + L[i]->getPeso2()*(lambda2);
// 			if (peso<=(1+num)*w){
// 				if (conjunto.compare(L[i]->getOrigem(), L[i]->getDestino())==false){
// 					ret.push_back(make_pair(L[i],i));
// 				} else {
// 					L[i] = NULL;
// 				}
// 			}
// 		}
// 	}
// 	return ret;
// }

// pair<int*, pair<float, float> > rmcKruskal(float lambda1, float lambda2,Aresta** presort, float num){
// 	pair<int*, pair<float, float> > ret = make_pair(new int[g.getQuantVertices()-1], make_pair(0,0)); 
// 	int cont = 0;
// 	Conjunto conjunto(g.getQuantVertices());
// 	for (int i=0; i<g.getQuantArestas() && arestasObrigatorias>0; i++){
// 		if (g.getStatus(presort[i]->getId())==1){ // se for obrigatoria
// 			Aresta *a_new = presort[i];
// 			ret.first[cont++]  = a_new->getId(); 
// 			ret.second.first  +=a_new->getPeso1();
// 			ret.second.second +=a_new->getPeso2();
// 			conjunto.union1(a_new->getOrigem(), a_new->getDestino());
// 			presort[i] = NULL;
// 		}
// 	}
// 	while (cont<g.getQuantVertices()-1){
// 		vector<pair <Aresta* , int > > restrictas = restricted_list(lambda1,lambda2, presort, num,conjunto);
// 		//cout<<"restrictas.size() = "<<restrictas.size()<<endl;
// 		if (restrictas.size()>0){
// 			int pos = rand()%restrictas.size();
// 			Aresta *a_new = restrictas[pos].first;
// 				conjunto.union1(a_new->getOrigem(), a_new->getDestino());
// 				ret.first[cont++] = a_new->getId(); 
// 				ret.second.first+=a_new->getPeso1();
// 				ret.second.second+=a_new->getPeso2();
// 				presort[restrictas[pos].second] = NULL;
// 		}
// 	}

// 	return ret;
// }


//retorna “verdadeiro” caso sl esteja em uma regiao menos populosa que s com relacao ao arquivo local_arc.
bool m_grid(Arquivo &local_arc, pair<int*, pair<float, float> > sl, pair<int*, pair<float, float> > s){
	return local_arc.getPositionCount(sl)<local_arc.getPositionCount(s);
}

//edita sol com uma solucao que minimiza o custo escalarizado
void localSearch(pair<int*, pair<float, float> >  &sol, float cateto_x, float cateto_y, float escalarX, float escalarY, Arquivo &arc){
	for (int i=0; i<maxSearch; i++){
		list <pair<int*,  pair<float, float> > > viz = vizinhos2(sol ,cateto_x, cateto_y, escalarX,escalarY,arc);
		list<pair<int*, pair<float, float> > >::iterator it = viz.begin();
		float min = (*it).second.first*escalarX + (*it).second.second*escalarY;
		bool modificou = false;
		for (list<pair<int*, pair<float, float> > >::iterator it3=viz.begin(); it3!=viz.end(); it3++){
			float custo = (*it3).second.first*escalarX + (*it3).second.second*escalarY;
			if (custo<min || m_grid(arc, (*it3), (*it))==true){
				min = custo;
				it = it3;
				modificou = true;
			}
		}
		if (modificou==true){
			//cout<<"Melhorou"<<endl;
			for (int kgo = 0; kgo<g.getQuantVertices()-1; kgo++){
				sol.first[kgo] = (*it).first[kgo];
			}
			sol.second.first = (*it).second.first;
			sol.second.second = (*it).second.second;
		}
	}
}

//IMPORTANTE, para o std::sort funcionar, precisa retornar false se os elementos forem idênticos
bool compare(pair<int*, pair<float, float> > p1, pair<int*, pair<float, float> > p2){
 	return (p1.second.first<p2.second.first) || (p1.second.first==p2.second.first && p1.second.second<p2.second.second);
}

// /*Diversification Generation Method : gera solucoes maxSolInit com o rmcKruskal que estejam no triângulo retângulo formado pelos catetos x e y*/
// vector < pair<int*, pair<float, float> > >  getPopulacaoInicial(float cateto_x, float cateto_y, float escalarX, float escalarY, Arquivo &arq){
// 	Aresta **arestasPtr22 = g.getAllArestasPtr();
// 	mergesort(0, escalarX,escalarY, 0, arestasPtr22, g.getQuantArestas(),3);
// 	Aresta **arestasPtr = new  Aresta*[g.getQuantArestas()];
// 	vector < pair<int*, pair<float, float> > >  popInicial;
// 	for (int i= 0; i<maxSolInit; i++){
// 		for (int kkk = 0; kkk<g.getQuantArestas(); kkk++) arestasPtr[kkk] = arestasPtr22[kkk];
// 		float num = ((float)(rand()%101))/1000.0; // 0...1000
// 		pair<int*, pair<float, float> > tree = rmcKruskal(escalarX,escalarY, arestasPtr,num);
// 		if (tree.second.first<cateto_x && tree.second.second<cateto_y) {
// 			arq.adicionarSol(clone(tree)); // nao dominadas
// 			popInicial.push_back(tree); // pode entrar solucoes dominadas
// 		} else {
// 			delete [] tree.first;
// 		}
// 	}
// 	delete[] arestasPtr;
// 	return popInicial;
// }



vector < pair<int*, pair<float, float> > > getReferenceSet(vector < pair<int*, pair<float, float> > > populacao){
	vector < pair<int*, pair<float, float> > > referece;
	for (int i=0; i<populacao.size(); i++){ // verifia se i é dominada por j. Se sim, nao insere
		bool isdominada = false;
		for (int j=0; j<populacao.size(); j++){
			if (i!=j){
				if (populacao[i].second.first<=populacao[j].second.first && populacao[i].second.second<=populacao[j].second.second && (populacao[i].second.first<populacao[j].second.first || populacao[i].second.second<populacao[j].second.second )){
					isdominada = true;
					break;
				}
			}
		}
		if (isdominada==false){
			referece.push_back(populacao[i]);
		}
	}
	return referece;
}

// //populacao deve estar ordenada lexicograficamente
// vector < pair<int*, pair<float, float> > > getReferenceSet_depreciate(vector < pair<int*, pair<float, float> > > populacao){
// 	vector < pair<int*, pair<float, float> > > referece;
// 	for (int i=0; i<referenceSetSize1; i++){
// 		referece.push_back(populacao[i]);
// 	}

// 	for (int vez = 0; vez<(referenceSetSize-referenceSetSize1); vez++){
// 		float maior = INT_MIN;
// 		pair<int*, pair<float, float> > mmmaior;
// 		for (int i=referenceSetSize1; i<populacao.size(); i++){
// 			float menor = INT_MAX;
// 			for (int j=0; j<referece.size(); j++){
// 				float dist = distancia(referece[j].second, populacao[i].second);
// 				if (dist<menor){
// 					menor = dist;
// 				}
// 			}
// 			if(menor>maior){
// 				maior = menor;
// 				mmmaior = populacao[i];
// 			}
// 		}
// 		if (maior!=INT_MIN){
// 			referece.push_back(mmmaior);
// 		}
// 	}
// 	return referece;
// }


void path_relinking(pair<int*, pair<float, float> > start, pair<float, float>  target,Arquivo &arq, float cateto_x, float cateto_y, float escalarX, float escalarY, vector < pair<int*, pair<float, float> > > &pop){
	float distanciaOriginal = 0;
	int contMax = 0;
	pair<int*, pair<float, float> > startaux = clone(start);
	int contttt=0;
	do{
		//cout<<"\t \t contMax = "<<contMax<<endl;
		distanciaOriginal = distancia(startaux.second, target);
		//cout<<"\t \t distanciaOriginal = "<<distanciaOriginal<<endl;
		if (distanciaOriginal>0){
			//vizinhos que dominam extritamente 'startaux' e que se aproximam do target
			list <pair<int*,  pair<float, float> > > vizi =  vizinhos2(startaux ,cateto_x, cateto_y, escalarX,escalarY, arq);
			vector <pair<int*,  pair<float, float> > > vizi2;
			for (list <pair<int*,  pair<float, float> > >::iterator in=vizi.begin(); in!=vizi.end(); in++){
				if (distancia((*in).second, target)<distanciaOriginal) vizi2.push_back((*in)); //|| m_grid(arq, (*in),startaux)==true
			}
			if (vizi2.size()>0){

				pair<int*, pair<float, float> > proximooo = vizi2[rand()%vizi2.size()]; // sortea o proximo a ser verificado
				contMax++;
				for (int i=0; i<g.getQuantVertices(); i++) startaux.first[i] = proximooo.first[i];
				startaux.second.first = proximooo.second.first;
				startaux.second.second = proximooo.second.second;
			} else {
				contMax++;
				//break; // sai do laço
			}
		}
	} while (distanciaOriginal > 0 && contMax<maxPR);
	//cout<<contMax<<endl;
	delete[] startaux.first;


}


pair<int*, pair<float, float> > initPopAux(float lambda1, float lambda2, bool &isvalido){
	Aresta **arestasPtr = g.getAllArestasPtr();
	
	mergesort(0, lambda1,lambda2, 0, arestasPtr, g.getQuantArestas(),3);
	int *nova = new int[g.getQuantVertices()-1];
		
	float peso1=0, peso2=0;
	int cont = 0;
	Conjunto conc(g.getQuantVertices());
	vector<Aresta *> amostral;
	for (int i=0; i<g.getQuantArestas(); i++){
		Aresta *a = arestasPtr[i];
		if (g.getStatus(a->getId())==1){ // obrigatoria
			nova[cont++] = a->getId();
			conc.union1(a->getOrigem(), a->getDestino());
			peso1+= a->getPeso1();
			peso2+= a->getPeso2();
		} else {
			amostral.push_back(a);
		}
	}
	int rlc = alpha*amostral.size();
	if (rlc<1) rlc = 1;
		//cout<<"rlc = "<<rlc<<"  amostral.size() = "<<amostral.size()<<endl;
	while (cont<g.getQuantVertices()-1 && rlc>0){
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
	if (cont<g.getQuantVertices()-1){ // se por acaso, o laço acima terminar sem uma arvore estar formada, entao adcionamos as menores arestas
		for (int i=0; i<amostral.size() && cont<g.getQuantVertices()-1; i++){
			Aresta *a = amostral[i];
			if (conc.compare(a->getOrigem(),a->getDestino())==false){
				nova[cont++] = a->getId();
				conc.union1(a->getOrigem(),a->getDestino());
				peso1+= a->getPeso1();
				peso2+= a->getPeso2();
			} 
		}
	}
	if (cont==g.getQuantVertices()-1){
		isvalido = true;
	} else isvalido = false;

	pair<int*, pair<float, float> > sol1 = make_pair(nova, make_pair(peso1,peso2));
	return sol1;	
}


pair<int*, pair<float, float> > recombinacao(float lambda1, float lambda2, bool &isvalido, pair<int*, pair<float, float> > s1, pair<int*, pair<float, float> > s2){
	//Aresta **arestasPtr = g.getAllArestasPtr();
	int size = g.getQuantVertices()-1+g.getQuantVertices()-1;
	int extra = 1;
	Aresta **arestasPtr = new Aresta*[size+extra];
	for (int i=0; i<g.getQuantVertices()-1; i++){
		arestasPtr[i] = g.getArestas(s1.first[i]);
		arestasPtr[i+g.getQuantVertices()-1] =  g.getArestas(s2.first[i]);
	}
	arestasPtr[extra]   = g.getArestas(rand()%g.getQuantArestas());
	
	mergesort(0, lambda1,lambda2, 0, arestasPtr, size,3);
	int *nova = new int[g.getQuantVertices()-1];
		
	float peso1=0, peso2=0;
	int cont = 0;
	Conjunto conc(g.getQuantVertices());
	vector<Aresta *> amostral;
	for (int i=0; i<size; i++){
		Aresta *a = arestasPtr[i];
		if (g.getStatus(a->getId())!=0 && conc.compare(a->getOrigem(), a->getDestino())==false){ // obrigatoria
			nova[cont++] = a->getId();
			conc.union1(a->getOrigem(), a->getDestino());
			peso1+= a->getPeso1();
			peso2+= a->getPeso2();
		} else if (conc.compare(a->getOrigem(), a->getDestino())==false){
			amostral.push_back(a);
		}
	}
	int rlc = alpha*amostral.size();
	if (rlc<1) rlc = 1;
		//cout<<"rlc = "<<rlc<<"  amostral.size() = "<<amostral.size()<<endl;
	while (cont<g.getQuantVertices()-1 && rlc>0){
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
	if (cont<g.getQuantVertices()-1){ // se por acaso, o laço acima terminar sem uma arvore estar formada, entao adcionamos as menores arestas
		for (int i=0; i<amostral.size() && cont<g.getQuantVertices()-1; i++){
			Aresta *a = amostral[i];
			if (conc.compare(a->getOrigem(),a->getDestino())==false){
				nova[cont++] = a->getId();
				conc.union1(a->getOrigem(),a->getDestino());
				peso1+= a->getPeso1();
				peso2+= a->getPeso2();
			} 
		}
	}
	if (cont==g.getQuantVertices()-1){
		isvalido = true;
	} else isvalido = false;

	pair<int*, pair<float, float> > sol1 = make_pair(nova, make_pair(peso1,peso2));
	return sol1;	
}



/*Diversification Generation Method : gera solucoes maxSolInit com o rmcKruskal que estejam no triângulo retângulo formado pelos catetos x e y*/
vector < pair<int*, pair<float, float> > >  getPopulacaoInicial(float xp, float yp,float  xq, float yq, Arquivo &arq){
	/*assume que xp < xq e yq < yp*/
	vector < pair<int*, pair<float, float> > >  popInicial;
	float lam1 = (yp-yq);
	float lam2 = (xq-xp);
	float menor = lam1<lam2 ? lam1 : lam2;
	float taxa = (float) menor/maxSolInit;
	for (int i=0; i<maxSolInit; i++){
		bool resposta;
		pair<int*, pair<float, float> > tree = initPopAux((yp-yq),(xq-xp), resposta);
		if (resposta==true){
			popInicial.push_back(tree);
			arq.adicionarSol(clone(tree));
		}	else{
			cout<<"FALLLLSE INIT"<<endl;
			delete[] tree.first;
		}	
		xq+=taxa;
		yp+=taxa;
	}
	return popInicial;

}

// /* 
// Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
// Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
// Assim, teremos separadas as solucoes suportadas e nao suportadas

// quantSols recebe apenas a quantidade de todas as solucoes nao suportadas
// */
list < pair<int*, pair<float, float> > > phase2KB(list< pair<int*, pair<float, float> > > &extremas){ 
	extremas.sort(compare);
	list < pair<int*, pair<float, float> > > noSoportadas; // resultado a ser retornado
	list< pair<int*, pair<float, float> > >::iterator it = extremas.begin(); 
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		Arquivo noSuportadasPQ;
		pair<int*, pair<float, float> > ponto_p = *(it);
		pair<int*, pair<float, float> > ponto_q = *(++it);
		int *p = ponto_p.first; 
		int *q = ponto_q.first;
		
		xp = ponto_p.second.first;
		yp = ponto_p.second.second;
		xq = ponto_q.second.first;
		yq = ponto_q.second.second;
		
		vector < pair<int*, pair<float, float> > > populacao =getPopulacaoInicial(xp, yp,  xq, yq,noSuportadasPQ);
		int contScatter = 0;
		for (int i=0; i<populacao.size(); i++){
			localSearch(populacao[i], xq, yp, (yp-yq), (xq-xp), noSuportadasPQ);
		}
		populacao.push_back(ponto_p);
		populacao.push_back(ponto_q);
		//cout<<"Triângulo = "<<contador<<endl;
		do{	
			//cout<<"\tcontScatter = "<<contScatter<<endl;
			//cout<<contScatter<<" populacao = "<<populacao.size()<<endl;
			vector < pair<int*, pair<float, float> > > reference = getReferenceSet(populacao);
			//cout<<"reference.size() = "<<reference.size()<<endl;
			//populacao.clear();
			for (int iiii=0; iiii<maxSubsets && reference.size()>0 && noSuportadasPQ.getSize()<(xq-xp-1) && noSuportadasPQ.getSize()<(yp-yq-1) ; iiii++){//noSuportadasPQ.getSize()<limiteSols
				pair<int*, pair<float, float> > tr1 = reference[rand()%reference.size()];
				pair<int*, pair<float, float> > tr2 = reference[rand()%reference.size()];
				
				float escalarX = abs(tr1.second.second - tr2.second.second);
				float escalarY = abs(tr1.second.first - tr2.second.first);
				
				bool resposta;
				pair<int*, pair<float, float> > tree = recombinacao(escalarX,escalarY, resposta, tr1, tr2);
				if (resposta==true){

					localSearch(tree, xq, yp, (yp-yq), (xq-xp), noSuportadasPQ);
	
					if (tree.second.first<xq && tree.second.second<yp) {
						path_relinking(tr1, tree.second,noSuportadasPQ , xq, yp, (yp-yq), (xq-xp),reference);
						path_relinking(tree, tr2.second,noSuportadasPQ , xq, yp, (yp-yq), (xq-xp),reference);
					
						noSuportadasPQ.adicionarSol(clone(tree)); // nao dominadas
						populacao.push_back(tree); // pode entrar solucoes dominadas
						reference.push_back(tree);
					} else {
						//delete [] tree.first;
						globalSolutions.adicionarSol22(tree);
					}
				} else {
					//cout<<"FALLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLSE"<<endl;
					delete [] tree.first;
				}
			}
			contScatter++;
		} while (contScatter<maxIteracoes && noSuportadasPQ.getSize()<(xq-xp-1) && noSuportadasPQ.getSize()<(yp-yq-1));// && noSuportadasPQ.getSize()<limiteSols); 
					
		list<pair<int*, pair<float, float> > > solsss = noSuportadasPQ.getSolucoes();
	//	cout<<"\tnoSuportadasPQ.size() = "<<solsss.size()<<endl;
		for (list<pair<int*, pair<float, float> > >::iterator soooslkd =solsss.begin(); soooslkd!=solsss.end(); soooslkd++){
			noSoportadas.push_back(clone((*soooslkd)));

		}
		contador++;
		

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
	//Grafo (my_grafo)(n);
	g.setN(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		g.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		g.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}
	int nA = id; // quantidade de arestas do grafo	
	
	 g.excluiProibidas(); // primeiro, excluimos as proibidas
	 g.updateIndex(); // depois, atualizamos os idexes das arestas no map
	obrigatorias = g.marcaObrigatorias(arestasObrigatorias); // determinanmos as obrigatorias PARAMETRO IMPORTANTE
	nA= g.getQuantArestas();

 	times(&tempsInit);

	 list <pair<int*, pair<float, float> > > arvores = suportadas();
	
	cout<<"Fim da primeira fase. Quantidade de solucoes suportadas : "<<arvores.size()<<endl;
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	//cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);

	list <pair<int*, pair<float, float> > > noSuportadas = phase2KB(arvores);
	
	cout<<"Fim da segunda fase. Quantidade de solucoes nao-suportadas: "<<noSuportadas.size()<<endl;
	
	times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	cout<<"Total antes = "<<noSuportadas.size() + arvores.size()<<endl;
	cout<<"Total fora = "<<globalSolutions.getSize()<<endl;
	

	for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		globalSolutions.adicionarSol22(*it);
	}


	for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		globalSolutions.adicionarSol22(*it);
	}

	noSuportadas = globalSolutions.getSolucoes();
	cout<<"Quantidade total de solucoes : "<<noSuportadas.size()<<endl;
	
	cout<<"Total: ";
	cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<" segundos"<<endl;
	//cout<<"Total fora : "<<globalSolutions.getSize()<<endl;

	FILE *paretoFront = fopen(argv[1],"a");
   	FILE *tempofile = fopen(argv[2],"a");
   

   	fprintf(tempofile,"%.10lf\n", (float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK));
   	

	
    int i = 1, cont=0;
 	float max_ob1 = -1, max_ob2 = -1;
    
	//cout<<"Resultado \n SUPORTADS E NAO SUPORTADAS"<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		float cont1=0, cont2=0;
		pair<int*, pair<float, float> > arvr = (*it);
		Conjunto conjjj(n);
   		for (int a = 0; a<n-1; a++){ // cada aresta da arvore
			int iddd = (arvr.first)[a];
				if (conjjj.compare(g.getArestas(iddd)->getOrigem(), g.getArestas(iddd)->getDestino())==false){
					
					cont1+=g.getArestas(iddd)->getPeso1();
					cont2+=g.getArestas(iddd)->getPeso2();
					conjjj.union1(g.getArestas(iddd)->getOrigem(), g.getArestas(iddd)->getDestino());
    		
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