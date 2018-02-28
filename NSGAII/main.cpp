/* 
=====================================================================================

Copyright Islame Felipe da COSTA FERNANDES (2018)
	
This code implements a NSGAII algorithm for Bi-objective spanning tree 
The data structure and some functions were kindly provided by Monteiro (2010)


=====================================================================================
*/

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sys/times.h>
#include <sys/types.h>
#include <iostream>
#include <math.h>       /* acos, cos */
#include <climits>

#include "BoundedParetoSet.cpp"
#include "SolucaoEdgeSet.cpp"
#include "rmcPrim.cpp"
#include "popInicial.cpp"


using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
SolucaoEdgeSet *populacao[TAMANHOPOPULACAO];
BoundedParetoSet arc_global;
int objetivoOrdenacao; // esta variavel é utilizada para designar qual objetivo será utilizado para ordenar as soluçoes

struct tms tempoAntes, tempoDepois;

void input(){
	int n; // esta leitura de "n" é somente para cumprir o formato da instância. Os valores de fato estao em param.h
	cin>>n; // quantidade de vertices

	int org, dest;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			cin>>org;
			cin>>dest;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				cin>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j]; // a parte superior da matriz é refletida na inferior
			}
		}
	}
}

// deb et al (2002)
// P[TAMANHOPOPULACAO*2] = populacao corrente unido com a populaçao criada
// tem que ser EXATAMENTE TAMANHOPOPULACAO*2
// F é o vetor de fronts. Cada front (f0, f1, f2, ... ) é uma list. A quantidade maxima de fronts é TAMANHOPOPULACAO*2
void fast_non_dominanted_sort(SolucaoEdgeSet *P[TAMANHOPOPULACAO*2], list<SolucaoEdgeSet *> F[TAMANHOPOPULACAO*2], int &quantFronts){
	list< SolucaoEdgeSet* > S[TAMANHOPOPULACAO*2]; // para cada p \in P, guarda a lista de soluçoes dominadas por p
	int N[TAMANHOPOPULACAO*2]; // para cada p \in P, guarda a quantidade de soluçoes que dominam p
	// int prank[TAMANHOPOPULACAO*2]; // para cada p \in P, guarda o rank de p
	quantFronts = 0;
	for (int p=0; p<TAMANHOPOPULACAO*2; p++){
		N[p] = 0;
		// prank[p] = 0;
	}
	for (int p=0; p<TAMANHOPOPULACAO*2; p++){
		P[p]->posicaoListaNSGAII = p;
		for (int q=0; q<TAMANHOPOPULACAO*2; q++){
			if (*P[p]>>*P[q]){
				S[p].push_back(P[q]);
			} else if (*P[q]>>*P[p]){
				N[p] = N[p] + 1;
			}
		}

		if (N[p] == 0){
			// prank[p] = 0; // primeiro rank. RANK COMEÇA DO 0 
			F[0].push_back(P[p]); // primeiro rank. RANK COMEÇA DO 0
		}
	}
	int i=0; 
	while (F[i].size()>0){
		for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
			int pp = (*p)->posicaoListaNSGAII;
			for (list< SolucaoEdgeSet* >::iterator q = S[pp].begin(); q!=S[pp].end(); q++){
				int qq = (*q)->posicaoListaNSGAII;
				N[qq] = N[qq] - 1;
				if (N[qq] == 0){ //q vai pro próximo rank
					F[i+1].push_back((*q));
				}
			}
		}
		i++;
	}
	quantFronts = i;

}

// ferifica se a lista de Fronts está correta: apenas para fins de verificaçao da corretude
bool verificador(list<SolucaoEdgeSet *> F[TAMANHOPOPULACAO*2], int sizeFront){
	for (int i=0; i<sizeFront; i++){
		for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
			for (list< SolucaoEdgeSet* >::iterator pp = F[i].begin(); pp!=F[i].end(); pp++){
				if (p!=pp){
					if (**p>>**pp || **pp>>**p) {
						cout<<"RROOO 1"<<endl;
						return false;
					}
				}
			}
			bool encontrou = false;
			for (int ii = i-1; ii>=0 && encontrou==false; ii--){
				for (list< SolucaoEdgeSet* >::iterator pp = F[ii].begin(); pp!=F[ii].end() && encontrou==false; pp++){
					if (**pp>>**p) encontrou = true;
				}
			}	
			if (i!=0 && encontrou==false){
				cout<<"RROOO 2 "<<endl;//i = "<<i<<" ==> "<<(*p)->getObj(0)<<" "<<(*p)->getObj(1)<<endl;
				return false;
			} 

		}
	}
	return true;
}

int main(int argc, char *argv[]){
	int seemente = std::atoi(argv[1]);
	init_genrand64(seemente);
	cout<<"========= Estatisticas ========= "<<endl;
	cout<<"Semente utilizada : "<<seemente<<endl;
	FILE *samplefile = fopen(argv[2],"a");
	FILE *tempofile = fopen(argv[3],"a");
	input(); // ler instância
	cout<<"Instância lida..."<<endl;

	SolucaoEdgeSet *populacao2[TAMANHOPOPULACAO];// excluir
	SolucaoEdgeSet *uniao[TAMANHOPOPULACAO*2];// excluir
	alocaPopulacao(populacao);
	gerarPopulacao1(populacao);

	alocaPopulacao(populacao2); // excluir
	gerarPopulacao1(populacao2); // excluir

	for (int i=0; i<TAMANHOPOPULACAO*2; i++){
		uniao[i] = new SolucaoEdgeSet(NUMEROVERTICES-1);
		if (i<TAMANHOPOPULACAO){
			uniao[i] = populacao[i];
		} else {
			uniao[i] = populacao2[i-TAMANHOPOPULACAO];
		}
	}

	// int ns1 = 0;
	// int ns2 = 1;
	// SolucaoEdgeSet * s1 = populacao[ns1];
	// SolucaoEdgeSet * s2 = populacao[ns2];
	// SolucaoEdgeSet * novo = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// novo->crossover(*s1,*s2);
	// SolucaoEdgeSet * novo2 = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// novo2->mutacao(*novo);
	// s1->isTree();
	// s2->isTree();
	// novo->isTree();
	// novo2->isTree();

	// cout<<s1->getObj(0)<<" "<<s1->getObj(1)<<endl;
	// cout<<s2->getObj(0)<<" "<<s2->getObj(1)<<endl;
	// cout<<novo->getObj(0)<<" "<<novo->getObj(1)<<endl;
	// cout<<novo2->getObj(0)<<" "<<novo2->getObj(1)<<endl;

	int sizeFront = 0;
	list<SolucaoEdgeSet *> F[TAMANHOPOPULACAO*2];
	fast_non_dominanted_sort(uniao, F, sizeFront);
	for (int i=0; i<sizeFront; i++){
		cout<<"Front "<<i+1<<":"<<endl;
		for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
			cout<<"\t";
			cout<<(*p)->getObj(0)<<" "<<(*p)->getObj(1)<<endl;
		}
		cout<<endl;
	}
	cout<<"verificador = "<<verificador(F, sizeFront)<<endl;


}