#ifndef PATHRELINKING_CPP 
#define PATHRELINKING_CPP

#include <list>
#include <map>
#include "SolucaoEdgeSet.cpp"
#include "param.h"

using namespace std;

extern BoundedParetoSet *arquivoLimitadoGlobal;
int amostralSai[NUMEROVERTICES-1]; // guarda as arestas que podem sair de s1 (vide abaixo)
/*retorna a quantidade de arestas que existem em s1 que nao existem em s2*/
int distancia(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	int cont=0;
	for (int i=0; i<NUMEROVERTICES-1; i++){
		bool ha = false;
		for (int j=0; j<NUMEROVERTICES-1 && ha==false; j++){
			if ((s2->edges[j][0]==s1->edges[i][0] && s2->edges[j][1]==s1->edges[i][1]) || (s2->edges[j][0]==s1->edges[i][1] && s2->edges[j][1]==s1->edges[i][0])){
				ha = true;
			}
		}	
		if (ha==false) cont++;
	}
	return cont;
}

int getArestasSai(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	int cont=0;
	for (int i=0; i<NUMEROVERTICES-1; i++){
		bool ha = false;
		for (int j=0; j<NUMEROVERTICES-1 && ha==false; j++){
			if ((s2->edges[j][0]==s1->edges[i][0] && s2->edges[j][1]==s1->edges[i][1]) || (s2->edges[j][0]==s1->edges[i][1] && s2->edges[j][1]==s1->edges[i][0])){
				ha = true;
			}
		}	
		if (ha==false) {
			amostralSai[cont] = i;
			cont++;
		}
	}
	return cont;
}

/*Sorteia-se uma aresta e pra sair. Sorteia-se uma aresta e' que pode entrar tal que:
		e' nao é dominada por e
		e' nao forma ciclo
*/
std::vector<SolucaoEdgeSet *> getVizinho2(SolucaoEdgeSet *soloriginal, double distan, SolucaoEdgeSet *target){
	//soloriginal->isTree();
	SolucaoEdgeSet novoV(NUMEROVERTICES-1); // f[] = 0;
	novoV.uf.clear();

	// remove a aresta "e"
	int e2 = IRandom(0,distan-1);
	int e = amostralSai[e2];
	for (int i=0;i<NUMEROVERTICES-1;i++){
		if (i != e) {
			//cout<<soloriginal->edges[i][0]<<" "<<soloriginal->edges[i][1]<<endl;
			novoV.edges[i][0] = soloriginal->edges[i][0];
			novoV.edges[i][1] = soloriginal->edges[i][1];
			novoV.uf.unionClass(novoV.edges[i][0],novoV.edges[i][1]);
			for (int k=0;k<NUMOBJETIVOS;k++)
				novoV.incrementeF(k, f(k,novoV.edges[i][0],novoV.edges[i][1]));
		}
	}
	int amostral[NUMEROVERTICES*NUMEROVERTICES][2];
	int conttAmotral = 0; 
	// ATENCAO: GRAFO COMPLETO
	double obj0 = f(0, soloriginal->edges[e][0], soloriginal->edges[e][1]);
	double obj1 = f(1, soloriginal->edges[e][0], soloriginal->edges[e][1]);
	//cout<<"\n\n"<<endl;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			if ((obj0<=f(0,i,j) && obj1<=f(1,i,j) && (obj0<f(0,i,j) || obj1<f(1,i,j)))==false){
				if (soloriginal->edges[e][0]!=i && soloriginal->edges[e][1]!=j && novoV.uf.sameClass(i,j)==false){
					amostral[conttAmotral][0] = i;
					amostral[conttAmotral][1] = j;
					conttAmotral++;
				}
			}
		}
	} // olha todas as arestas que podem entrar 
	//cout<<"conttAmotral = "<<conttAmotral<<endl;
	std::vector<SolucaoEdgeSet *> ret;
	int confkgj = 0; 
	for (int i=0; i<conttAmotral; i++){
		novoV.edges[e][0] = amostral[i][0];
		novoV.edges[e][1] = amostral[i][1];
		for (int k=0;k<NUMOBJETIVOS;k++)
			novoV.incrementeF(k, f(k,novoV.edges[e][0],novoV.edges[e][1]));
		arquivoLimitadoGlobal->adicionarSol(&novoV);
		//cout<<novoV.getObj(0)<<" "<<novoV.getObj(1)<<endl;
		if (distancia(&novoV, target)<distan){
			ret.push_back(new SolucaoEdgeSet(NUMEROVERTICES-1));
			*ret[confkgj++] = novoV;
		}
		for (int k=0;k<NUMOBJETIVOS;k++)
			novoV.decrementeF(k, f(k,novoV.edges[e][0],novoV.edges[e][1]));
	}
	return ret;
}


//este procedimento nao retorna nada. 
//As soluçoes no path entre start e target sao inseridas automaticamente arquivoLimitadoGlobal 
void pathrelinking(SolucaoEdgeSet *start, SolucaoEdgeSet *target){
	float distanciaOriginal = 0; // distância entre duas soluçoes
	int contMax = 0; // cont do laço global do PR
	SolucaoEdgeSet *startaux = new SolucaoEdgeSet(NUMEROVERTICES-1); // copia
	*startaux = *start; // copia
	distanciaOriginal = getArestasSai(startaux, target); // arestas que estao em startaux que nao estao em target
	do{
		if (distanciaOriginal>0){
			cout<<"distanciaOriginal = "<<distanciaOriginal<<endl;
			//vizinhos que nao sao dominados por 'startaux' e que se aproximam do target
			std::vector<SolucaoEdgeSet * > vizinhos = getVizinho2(startaux, distanciaOriginal, target);
			cout<<"vizinhos.size() = "<<vizinhos.size()<<endl;
			if (vizinhos.size()>0){
				*startaux = *vizinhos[IRandom(0, vizinhos.size()-1)];
				distanciaOriginal = getArestasSai(startaux, target);
				contMax++;
			} else {
				contMax= maxPR+1; // sai do laço
			}
		}
	} while (distanciaOriginal > 0 && contMax<maxPR);
	cout<<"contMax = "<<contMax<<endl;
	delete startaux;
}


#endif