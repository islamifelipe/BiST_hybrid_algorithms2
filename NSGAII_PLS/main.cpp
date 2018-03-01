/* 
=====================================================================================

Copyright Islame Felipe da COSTA FERNANDES (2018)
	
This code implements a NSGAII+PLS algorithm for Bi-objective spanning tree 
beased on Párraga-Álava, Dorn e Inostroza-Ponta (2017)
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
// #include "rmcPrim.cpp"
#include "popInicial.cpp"


using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
SolucaoEdgeSet *populacao[TAMANHOPOPULACAO];
SolucaoEdgeSet *uniao[TAMANHOPOPULACAO*2];

int objetivoOrdenacao; // esta variavel é utilizada para designar qual objetivo será utilizado para ordenar as soluçoes

struct tms tempoAntes, tempoDepois;

/*
	ATENCAO: os autores nao deixam claro se algum tipo de arquivo externo é utilizado.
	Na verdade, parece, segundo alguns relatos vagos do artigo, que o pareto retornado refere-se à propria populaçao do NSGAII
*/

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

// OK
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
			P[p]->prank = 0; // primeiro rank. RANK COMEÇA DO 0 
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
					(*q)->prank = i+1;
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
				return false;
			} 

		}
	}
	for (int i=0; i<sizeFront; i++){
		for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
			if ((*p)->prank!=i) {
				return false;
			}
		}
	}
	return true;
}

bool compare1(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	return s1->getObj(objetivoOrdenacao) < s2->getObj(objetivoOrdenacao);
}

bool compare2(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	return s1->distance > s2->distance; // maior distância
}

void crownding_distance_assigment(list<SolucaoEdgeSet *> I){
	#define INF 1e9
	int l = I.size();
	list<SolucaoEdgeSet *>::iterator it = I.begin();
	while (it!=I.end()){
		(*it)->distance = 0;
		it++;
	}
	for (int m=0; m<NUMOBJETIVOS; m++){ // para cada objetivo m
		objetivoOrdenacao = m;
		I.sort(compare1);
		(*I.begin())->distance = INF;
		(I.back())->distance = INF; // É ISSO MESMO: back() NAO retorna um ponteiro interator!!! 
		it = I.begin();
		it++; // começa do 1 
		for (int i = 1; i<(l-1); i++){
			it++; // pos
			double objPos = (*it)->getObj(m);
			it--; // volta para o original
			it--; //previous
			double objPrev = (*it)->getObj(m);
			it++; // volta para o original
			(*it)->distance = (*it)->distance + (objPos - objPrev);
			it++; // avança
		}
	}
	#undef INF
}

// atualiza a populaçao (global)
void atualizaPopulacaoNSGAII(SolucaoEdgeSet *novaPop[TAMANHOPOPULACAO]){
	for (int i=0; i<TAMANHOPOPULACAO*2; i++){
		if (i<TAMANHOPOPULACAO){
			*uniao[i] = *populacao[i];
		} else {
			*uniao[i] = *novaPop[i-TAMANHOPOPULACAO];
		}
	}
	int sizeFront = 0;
	list<SolucaoEdgeSet *> F[TAMANHOPOPULACAO*2];
	fast_non_dominanted_sort(uniao, F, sizeFront);
	int cont = 0;
	int i = 0;
	while (cont + F[i].size() < TAMANHOPOPULACAO && i<sizeFront){
		for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
			*populacao[cont++] = **p;
		}
		i++;
	}
	crownding_distance_assigment(F[i]);
	F[i].sort(compare2); // ordena por CD
	for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end() && cont<TAMANHOPOPULACAO; p++){
		*populacao[cont++] = **p;
	}

	cout<<"verificador = "<<verificador(F,sizeFront)<<endl;;
}

void NSGAII(){
	SolucaoEdgeSet * filho = new SolucaoEdgeSet(NUMEROVERTICES-1); //poderia ser global, pra otimizar;
	alocaPopulacao(populacao);
	gerarPopulacao1(populacao);
	int p1,p2,p3,p4;
	int pai, mae;	
	SolucaoEdgeSet *novaPop[TAMANHOPOPULACAO]; // cria-se uma populaçao de descentes
	alocaPopulacao(novaPop); 
	gerarPopulacao1(novaPop);
	atualizaPopulacaoNSGAII(novaPop); // ok

	for (int i=0; i<QUANTGERACOES; i++){ // para cada geraçao...
		cout<<"geracao "<<i+1<<endl;
		for (int j=0; j<TAMANHOPOPULACAO; j++){ // deve-se criar TAMANHOPOPULACAO novos individuos

			/*SORTEIA 4 individuos*/
			/*Faz-se o torneio binario entre eles*/
			p1 = IRandom(0, TAMANHOPOPULACAO-1);
			p2 = IRandom(0, TAMANHOPOPULACAO-1);
			p3 = IRandom(0, TAMANHOPOPULACAO-1);
			p4 = IRandom(0, TAMANHOPOPULACAO-1);
			// escolhe-se a de menor rank, ou, se ambas tiverem o mesmo rank, escolhe-se a de MAIOR distância de aglomeraçao
			if((populacao[p1]->prank<populacao[p2]->prank) || (populacao[p1]->prank==populacao[p2]->prank && populacao[p1]->distance>populacao[p2]->distance)){ // compete com o primeiro objetivo
				pai = p1;;
			} else {
				pai = p2;
			}
			if((populacao[p3]->prank<populacao[p4]->prank) || (populacao[p3]->prank==populacao[p4]->prank && populacao[p3]->distance>populacao[p4]->distance)){ // compete com o primeiro objetivo
				mae = p3;
			} else {
				mae = p4;
			}
			double p = genrand64_real3();
			if (p<TAXADECRUZAMENTO){
				filho->crossover(*populacao[pai], *populacao[mae]);
			} else {
				int sor = IRandom(0,1);
				if (sor==0) *filho = *populacao[pai];
				else *filho = *populacao[mae];
			}
			// // filho foi definido; Agora aplica-se mutaçao
			p = genrand64_real3();;//rg.Random();
			if (p<TAXADEMUTACAO){
				novaPop[j]->mutacao(*filho);
			} else{
				*novaPop[j] = *filho;
			}
		}
		atualizaPopulacaoNSGAII(novaPop);
	}
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
	for (int i=0; i<TAMANHOPOPULACAO*2; i++){
		uniao[i] = new SolucaoEdgeSet(NUMEROVERTICES-1); // permanente
	}


	times(&tempoAntes);

	NSGAII();

	times(&tempoDepois);


	fprintf(stdout,"Tempo(s) Final = %.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
	fprintf(tempofile,"%.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );

	for (int i=0; i<TAMANHOPOPULACAO; i++){
		fprintf(stdout,"%.10lf %.10lf\n",populacao[i]->getObj(0),populacao[i]->getObj(1));
	}
	for (int i=0; i<TAMANHOPOPULACAO; i++){
		fprintf(samplefile,"%.10lf %.10lf\n",populacao[i]->getObj(0),populacao[i]->getObj(1));
	}
	// arc_global.printSetPoints(stdout);
	// arc_global.printSetPoints(samplefile);

	fclose(samplefile);
	fclose(tempofile);



	// alocaPopulacao(populacao);
	// gerarPopulacao1(populacao);


	// SolucaoEdgeSet *populacao2[TAMANHOPOPULACAO];// excluir
	// SolucaoEdgeSet *uniao[TAMANHOPOPULACAO*2];// excluir
	// alocaPopulacao(populacao);
	// gerarPopulacao1(populacao);

	// alocaPopulacao(populacao2); // excluir
	// gerarPopulacao1(populacao2); // excluir
	// cout<<"Popualcao Antes : "<<endl;
	// for (int i=0; i<TAMANHOPOPULACAO; i++){
	// 	cout<<populacao[i]->getObj(0)<<" "<<populacao[i]->getObj(1)<<endl;
	// }
	// cout<<endl;
	// for (int i=0; i<TAMANHOPOPULACAO*2; i++){
	// 	uniao[i] = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// 	if (i<TAMANHOPOPULACAO){
	// 		*uniao[i] = *populacao[i];
	// 	} else {
	// 		*uniao[i] = *populacao2[i-TAMANHOPOPULACAO];
	// 	}
	// }

	// // int ns1 = 0;
	// // int ns2 = 1;
	// // SolucaoEdgeSet * s1 = populacao[ns1];
	// // SolucaoEdgeSet * s2 = populacao[ns2];
	// // SolucaoEdgeSet * novo = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// // novo->crossover(*s1,*s2);
	// // SolucaoEdgeSet * novo2 = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// // novo2->mutacao(*novo);
	// // s1->isTree();
	// // s2->isTree();
	// // novo->isTree();
	// // novo2->isTree();

	// // cout<<s1->getObj(0)<<" "<<s1->getObj(1)<<endl;
	// // cout<<s2->getObj(0)<<" "<<s2->getObj(1)<<endl;
	// // cout<<novo->getObj(0)<<" "<<novo->getObj(1)<<endl;
	// // cout<<novo2->getObj(0)<<" "<<novo2->getObj(1)<<endl;

	// int sizeFront = 0;
	// list<SolucaoEdgeSet *> F[TAMANHOPOPULACAO*2];
	// fast_non_dominanted_sort(uniao, F, sizeFront);
	// // for (int i=0; i<sizeFront; i++){
	// // 	cout<<"Front "<<i+1<<":"<<endl;
	// // 	for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
	// // 		cout<<"\t";
	// // 		cout<<(*p)->getObj(0)<<" "<<(*p)->getObj(1)<<endl;
	// // 	}
	// // 	cout<<endl;
	// // }
	// cout<<"verificador = "<<verificador(F, sizeFront)<<endl;
	// cout<<endl;
	// int cont = 0;
	// int i = 0;
	// while (cont + F[i].size() < TAMANHOPOPULACAO && i<sizeFront){
	// 	for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end(); p++){
	// 		*populacao[cont++] = **p;
	// 	}
	// 	i++;
	// }
	// crownding_distance_assigment(F[i]);
	// F[i].sort(compare2); // ordena por CD
	// for (list< SolucaoEdgeSet* >::iterator p = F[i].begin(); p!=F[i].end() && cont<TAMANHOPOPULACAO; p++){
	// 	*populacao[cont++] = **p;
	// }

	// cout<<"Popualcao depois : "<<endl;
	// for (int i=0; i<TAMANHOPOPULACAO; i++){
	// 	cout<<populacao[i]->getObj(0)<<" "<<populacao[i]->getObj(1)<<endl;
	// }

}