/* 
=====================================================================================

Copyright Islame Felipe da COSTA FERNANDES (2018)
	
This code implements a new hybrid transgenetic algorithmic for Bi-objective spanning tree 
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
// #include "vetoresDirecao.cpp"
// #include "simulatedannealing.cpp"
#include "Tabu.cpp"
#include "Plasmideo.cpp"
#include "primTransposon.cpp"
// #include "kruskalTransposon.cpp"

using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
SolucaoEdgeSet *populacao[TAMANHOPOPULACAO];
BoundedParetoSet arc_global;
int objetivoOrdenacao; // esta variavel é utilizada para designar qual objetivo será utilizado para ordenar as soluçoes

struct tms tempoAntes, tempoDepois;

int amostralARESTAVECTOR[NUMEROVERTICES*(NUMEROVERTICES-1)/2][2];
int contGeracoes = 0;
// quantPlas1, quantPlas2 e quantPlas3 sao utilizados pelo procedimento criarPlasmideos 
// e sao autilizados a cada INI_GER_SET geraçoes consecutivas
int quantPlas1; // quantidade (probabilidade de escolha) do plasmideo 1
int quantPlas2; // quantidade (probabilidade de escolha) do plasmideo 2
int quantPlas3; // quantidade (probabilidade de escolha) do plasmideo 3

int contPlasm1 = 0; // conta quantas vezes o plasmideo rmc-prim tenta atacar
int contSucessoPlasm1 = 0; // conta quantos sucessos o plasm1 teve em atacar
float sumMelhoriaPlasm1_obj1 = 0; // conta o quanto o plasm1 melhora o primeiro objetivo
float sumMelhoriaPlasm1_obj2 = 0; // conta o quanto o plasm1 melhora o segundo objetivo

int contPlasm2 = 0; // conta quantas vezes o plasmideo "de uma soluçao" tenta atacar
int contSucessoPlasm2= 0; // conta quantos sucessos o plasm2 teve em atacar
float sumMelhoriaPlasm2_obj1 = 0; // conta o quanto o plasm2 melhora o primeiro objetivo
float sumMelhoriaPlasm2_obj2 = 0; // conta o quanto o plasm2 melhora o segundo objetivo


int contPlasm3= 0; // conta quantas vezes o plasmideo "de duas soluçoes" tenta atacar
int contSucessoPlasm3= 0; // conta quantos sucessos o plasm3 teve em atacar
float sumMelhoriaPlasm3_obj1 = 0; // conta o quanto o plasm3 melhora o primeiro objetivo
float sumMelhoriaPlasm3_obj2 = 0; // conta o quanto o plasm3 melhora o segundo objetivo


int contPrimTrans= 0; // conta a quantidade e vezes em que o primTrans tenta atacar
int contSucessPrimTrans= 0; // conta a quantidade de sucesso que o primTrans obteve
float sumMelhoriaPrimTrans_obj1 = 0; // conta o quanto o PrimTrans melhora o primeiro objetivo 
float sumMelhoriaPrimTrans_obj2 = 0; // conta o quanto o PrimTrans melhora o segundo objetivo


int contBT = 0; // conta quantas vezes a BT foi invocada
int contSucessBT = 0; // conta quantos sucessos a BT teve
float sumMelhoriaBT_obj1 = 0; // conta o quanto a BT melhora o primeiro objetivo
float sumMelhoriaBT_obj2 = 0; // conta o quanto a BT melhora o segundo objetivo

int contRenovacoes = 0; // conta quantas vezes a populaçao foi renovada

// int quantPlas1_evolucao[10]; // memoriza a quantidade de plasmideo 1 construido a cada renovaçao 
// int quantPlas2_evolucao[10]; // momoriza a quantidade de plasmideo 2 construido a cada renovaçao
// int quantPlas3_evolucao[10]; // momoriza a quantidade de plasmideo 3 construido a cada renovaçao

int AVALICACOES;

FILE *samplefile;
FILE *tempofile;


void input(){
	int n; // esta leitura de "n" é somente para cumprir o formato da instância. Os valores de fato estao em param.h
	cin>>n; // quantidade de vertices
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				custos[ob][i][j] = 10e9;
				custos[ob][j][i] = 10e9;
			}
		}
	}
	int org, dest;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			cin>>org;
			cin>>dest;
			// org--;
			// dest--;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				cin>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j]; // a parte superior da matriz é refletida na inferior
			}
		}
	}
}


bool compare1(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	return s1->getObj(objetivoOrdenacao) < s2->getObj(objetivoOrdenacao);
}

bool compare2(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	return s1->distance > s2->distance; // maior distância
}

void criaPlasmideos(Plasmideo pls[NUMPLASMIDEOS]){
	int tam, cont=0, index2, index3;
	for (int i=0; i<quantPlas1 && cont<NUMPLASMIDEOS; i++){
		tam = IRandom(2,(int)(0.5*(NUMEROVERTICES-1)));// NOTAR: DIFERENTE DE SILVIA
		double randd = genrand64_real3();
		double lambda[2] = {randd, 1.0 - randd};
		pls[cont].geraPlasm_rmcPrim(lambda, tam);
		pls[cont].tipo = 1;
		cont++;
	}

	for (int i=0; i<quantPlas2 && cont<NUMPLASMIDEOS; i++){
		index2 = IRandom(0,arc_global.getSize()-1);
		tam = IRandom(2,(int)(0.5*(NUMEROVERTICES-1)));// NOTAR: DIFERENTE DE SILVIA
		pls[cont].geraPlasm_Solucao(*arc_global.getSolucao(index2), tam);
		pls[cont].tipo = 2;
		cont++;
	}

	for (int i=0; i<quantPlas3 && cont<NUMPLASMIDEOS; i++){
		index3 = IRandom(0,arc_global.getSize()-1);
		index2 = IRandom(0,arc_global.getSize()-1);
		tam =IRandom(2,(int)(0.5*(NUMEROVERTICES-1))); // NOTAR: DIFERENTE DE SILVIA
		pls[cont].geraPlasTwoSolutions(*arc_global.getSolucao(index2), *arc_global.getSolucao(index3), tam);
		pls[cont].tipo = 3;
		cont++;
	}
}

void computeContPlas(int numPlas){ // computa quantas vezes cada plasmideo atacou
	if (numPlas == 1){
		contPlasm1++;
	} else if (numPlas == 2){
		contPlasm2++;
	} else if (numPlas == 3){ // 
		contPlasm3++;
	}
}

void computeContSucss(int numPlas, SolucaoEdgeSet *antes, SolucaoEdgeSet *depois){ // computa quantos ataques foram bem sucedidos para cada palsmideo
	if (numPlas == 1){
		contSucessoPlasm1++;
		sumMelhoriaPlasm1_obj1 += depois->getObj(0) < antes->getObj(0) ? antes->getObj(0) - depois->getObj(0) : 0;
		sumMelhoriaPlasm1_obj2 += depois->getObj(1) < antes->getObj(1) ? antes->getObj(1) - depois->getObj(1) : 0;
	} else if (numPlas == 2){
		contSucessoPlasm2++;
		sumMelhoriaPlasm2_obj1 += depois->getObj(0) < antes->getObj(0) ? antes->getObj(0) - depois->getObj(0) : 0;
		sumMelhoriaPlasm2_obj2 += depois->getObj(1) < antes->getObj(1) ? antes->getObj(1) - depois->getObj(1) : 0;
	} else if (numPlas == 3){ // 
		contSucessoPlasm3++;
		sumMelhoriaPlasm3_obj1 += depois->getObj(0) < antes->getObj(0) ? antes->getObj(0) - depois->getObj(0) : 0;
		sumMelhoriaPlasm3_obj2 += depois->getObj(1) < antes->getObj(1) ? antes->getObj(1) - depois->getObj(1) : 0;

	}
}

void crowndDistance(){ // by Felipe
		#define INF 1e9
		ParetoSet nds; 

		list<SolucaoEdgeSet *> lixeira1 = arc_global.lixeira;
		// cout<<"ANTES = "<<lixeira1.size()<<endl;
		list<SolucaoEdgeSet *>::iterator it = lixeira1.begin();
		// cout<<"Antes"<<endl;
		while (it!=lixeira1.end()){
			nds.adicionarSol((*it)); // Hudson pega somente incomparáveis
			it++; 
		}
		// cout<<"DEPOIS"<<endl;
		//list<SolucaoEdgeSet *> arqq  = arc_global.getElementos();
		//lixeira.splice(lixeira.end(), arqq); // une a lixeira ao arquivo externo,
		list<SolucaoEdgeSet *> lixeira = nds.getElementos();
		// cout<<"DEPOIS 1 = "<<lixeira.size()<<endl;	
		double randd = genrand64_real3();
		double lambda[2] = {randd, 1.0 - randd};
		int l = lixeira.size();
		// cout<<"Tamanho da lixeira = "<<l<<endl;
		it = lixeira.begin();
		while (it!=lixeira.end()){
			if (genrand64_real3() < 0.5) {
				contBT++;
				float oobj1_antes = (*it)->getObj(0);
				float oobj2_antes = (*it)->getObj(1);
				if (buscaTabu((*it), lambda)) {
					contSucessBT++;
					float oobj1_depois = (*it)->getObj(0);
					float oobj2_depois = (*it)->getObj(1);
					sumMelhoriaBT_obj1 += (oobj1_depois < oobj1_antes)? oobj1_antes - oobj1_depois : 0;
					sumMelhoriaBT_obj2 += (oobj2_depois < oobj2_antes)? oobj2_antes - oobj2_depois : 0;
				}
				 // experimentos preliminares comprovaram que aqui é melhor
				/*Justificativa da probabilidade de 50% : com esta probabiliade, é possível mesclar 
				soluçoes intensificadas (melhores) e soluçoes nao-intensificadas*/
			}
			(*it)->distance = 0;
			it++;
		}
		// cout<<"fim BT"<<endl;
		
		for (int m=0; m<NUMOBJETIVOS; m++){ // para cada objetivo m
			objetivoOrdenacao = m;
			lixeira.sort(compare1);
			(*lixeira.begin())->distance = INF;
			(lixeira.back())->distance = INF; // É ISSO MESMO: back() NAO retorna um ponteiro interator!!! 
			it = lixeira.begin();
			it++; // começa do 1 (2 do Hudson)
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
			// termina no l-2 (end-1) ou (l-1 do Hudson)
		}
		
	
		lixeira.sort(compare2);
		it = lixeira.begin();
		int contt = 0;
		while (it!=lixeira.end() && contt < TAMANHOPOPULACAO){ // preenche populacao
			
			*populacao[contt] = *(*it); /// com * mesmo !!!
			contt++;
			it++;
		}
		nds.clear();
		arc_global.clearLixeira();
		#undef INF
	}

void transgenetic(){
	double prob_vectorr = PROB_VECTOR;
	alocaPopulacao(populacao);
	gerarPopulacao1(populacao);
	quantPlas1 = round(NUMPLASMIDEOS/3); // valor inicial
	quantPlas2 = round(NUMPLASMIDEOS/3); // valor inicial
	quantPlas3 = round(NUMPLASMIDEOS/3); // valor inicial
	// quantPlas1_evolucao[contRenovacoes] = quantPlas1;
	// quantPlas2_evolucao[contRenovacoes] = quantPlas2;
	// quantPlas3_evolucao[contRenovacoes] = quantPlas3;
	PrimTransposon ptrans;
	int conttt = 0;
	//arc_global.contQuantAvaliacoes<AVALICACOES
	for (int i=0; arc_global.contQuantAvaliacoes<AVALICACOES; i++){  // para cada geraçao INFINITO
		// cout<<arc_global.contQuantAvaliacoes<<endl;
		Plasmideo pls[NUMPLASMIDEOS];
		criaPlasmideos(pls); /// cria plasmideos
		//cout<<"Geracao "<<i+1<<endl;
		for (int pppt = 0; pppt < TAMANHOPOPULACAO; pppt++){ // cada individuo sofre um ataque de plasmideo ou de transpon

			if(genrand64_real3() < prob_vectorr){ // um plasmideo ataca
				// escolhe-se um plasmideo
				
				int indexPlas = IRandom(0,NUMPLASMIDEOS-1); // escolhe o plasmideo
				computeContPlas(pls[indexPlas].tipo); // incrementa a quantidade de escolha deste palsmideo
				SolucaoEdgeSet *copia = new SolucaoEdgeSet(NUMEROVERTICES-1);
				*copia = *populacao[pppt];
				pls[indexPlas].atacaSolucao(*copia);
				bool resul = arc_global.adicionarSol(copia);
				if (*copia >> *populacao[pppt] || resul == true){ // critério de aceitaçao
					//populacao[pppt] --> antes
					//copia --> depois
					computeContSucss(pls[indexPlas].tipo, populacao[pppt], copia); // computa aqui contSucessoPlasm1, contSucessoPlasm2 e contSucessoPlasm3
					*populacao[pppt] = *copia;
				}
				delete copia;;
			} else { // um transponsson ataca
				// escolhe-se um transponsson
				SolucaoEdgeSet *copia = new SolucaoEdgeSet(NUMEROVERTICES-1);
				*copia = *populacao[pppt];
				double randd = genrand64_real3();
				double lambda[2] = {randd, 1.0 - randd};
				ptrans.atacaSolucao(*copia,lambda);
				contPrimTrans++;
				bool resul = arc_global.adicionarSol(copia);
				if (*copia >> *populacao[pppt] || resul == true){
					//populacao[pppt] --> antes
					//copia --> depois
					sumMelhoriaPrimTrans_obj1 += copia->getObj(0) < populacao[pppt]->getObj(0) ? populacao[pppt]->getObj(0) -  copia->getObj(0): 0;
					sumMelhoriaPrimTrans_obj2 += copia->getObj(1) < populacao[pppt]->getObj(1) ? populacao[pppt]->getObj(1) -  copia->getObj(1): 0;
					
					*populacao[pppt] = *copia;
					contSucessPrimTrans++;

				}
				delete copia;; 
			}
		}

		conttt++;
		if (conttt == INI_GER_SET){
			
			contRenovacoes++;
			// redefine as quantidades individuais de cada plasmideo e cada transponsson
			// incrementa a probabilidade (PROB_VECTOR) de escolha do grupo de vetor que deve atacar
			// recicla a populaçao
			quantPlas1 = (int) round((float)contSucessoPlasm1/(contSucessoPlasm1 + contSucessoPlasm2 + contSucessoPlasm3)*NUMPLASMIDEOS);
			quantPlas2 = (int) round((float)contSucessoPlasm2/(contSucessoPlasm1 + contSucessoPlasm2 + contSucessoPlasm3)*NUMPLASMIDEOS);
			quantPlas3 = NUMPLASMIDEOS - quantPlas1 - quantPlas2;
			
			// cout<<"quantPlas1 = "<<quantPlas1<<" quantPlas2 = "<<quantPlas2<<" quantPlas3 = "<<quantPlas3<<endl;
			// cout<<"contSucessoPlasm1 = "<<contSucessoPlasm1<<" contSucessoPlasm2 = "<<contSucessoPlasm2<<" contSucessoPlasm3 = "<<contSucessoPlasm3<<endl;
			// quantPlas1_evolucao[contRenovacoes] = quantPlas1;
			// quantPlas2_evolucao[contRenovacoes] = quantPlas2;
			// quantPlas3_evolucao[contRenovacoes] = quantPlas3;

			prob_vectorr += PROB_FACTOR;
			// cout<<"Lixeira = "<<arc_global.lixeira.size()<<endl;
			// cout<<"Init = "<<arc_global.lixeira.size()<<endl;
			crowndDistance();
			// cout<<"Fim"<<endl;
			conttt = 0;


		}
		contGeracoes++;
	}
}


void printResultado(){

	times(&tempoDepois);
	cout<<"Quantidade de geracoes = "<<contGeracoes<<endl;
	cout<<"Quantidade de renovacoes = "<<contRenovacoes<<endl;
	// cout<<"Quantidade de plasm1 definido a cada renovacao = ";
	// for (int i=0; i<contRenovacoes+1; i++) cout<<quantPlas1_evolucao[i]<<",";
	// cout<<endl;
	// cout<<"Quantidade de plasm2 definido a cada renovacao = ";
	// for (int i=0; i<contRenovacoes+1; i++) cout<<quantPlas2_evolucao[i]<<",";
	// cout<<endl;
	// cout<<"Quantidade de plasm3 definido a cada renovacao = ";
	// for (int i=0; i<contRenovacoes+1; i++) cout<<quantPlas3_evolucao[i]<<",";
	cout<<endl;
	cout<<"Quantidade total de ataque dos plasm1 = "<<contPlasm1<<endl;
	cout<<"Quantidade total de sucesso do plasm1 = "<<contSucessoPlasm1<<endl;
	cout<<"Média de melhoria do plasm1 para o obj1 = "<<(contPlasm1==0?0:(float)sumMelhoriaPlasm1_obj1/contPlasm1)<<endl;;
	cout<<"Média de melhoria do plasm1 para o obj2 = "<<(contPlasm1==0?0:(float)sumMelhoriaPlasm1_obj2/contPlasm1)<<endl;;

	cout<<"Quantidade total de ataque dos plasm2 = "<<contPlasm2<<endl;
	cout<<"Quantidade total de sucesso do plasm2 = "<<contSucessoPlasm2<<endl;
	cout<<"Média de melhoria do plasm2 para o obj1 = "<<(contPlasm2==0?0:(float)sumMelhoriaPlasm2_obj1/contPlasm2)<<endl;;
	cout<<"Média de melhoria do plasm2 para o obj2 = "<<(contPlasm2==0?0:(float)sumMelhoriaPlasm2_obj2/contPlasm2)<<endl;;

	cout<<"Quantidade total de ataque dos plasm3 = "<<contPlasm3<<endl;
	cout<<"Quantidade total de sucesso do plasm3 = "<<contSucessoPlasm3<<endl;
	cout<<"Média de melhoria do plasm3 para o obj1 = "<<(contPlasm3==0?0:(float)sumMelhoriaPlasm3_obj1/contPlasm3)<<endl;;
	cout<<"Média de melhoria do plasm3 para o obj2 = "<<(contPlasm3==0?0:(float)sumMelhoriaPlasm3_obj2/contPlasm3)<<endl;;


	cout<<"Quantidade total de ataque dos primTrans = "<<contPrimTrans<<endl;
	cout<<"Quantidade total de sucesso do primTrans = "<<contSucessPrimTrans<<endl;
	cout<<"Média de melhoria do primTrans para o obj1 = "<<(contPrimTrans==0?0:(float)sumMelhoriaPrimTrans_obj1/contPrimTrans)<<endl;;
	cout<<"Média de melhoria do primTrans para o obj2 = "<<(contPrimTrans==0?0:(float)sumMelhoriaPrimTrans_obj2/contPrimTrans)<<endl;;

	cout<<"Quantidade total de invocacao da BT = "<<contBT<<endl;
	cout<<"Quantidade total de sucesso da BT = "<<contSucessBT<<endl;
	cout<<"Média de melhoria da BT para o obj1 = "<<(contBT==0?0:(float)sumMelhoriaBT_obj1/contBT)<<endl;;
	cout<<"Média de melhoria da BT para o obj2 = "<<(contBT==0?0:(float)sumMelhoriaBT_obj2/contBT)<<endl;;

	cout<<"Quantidade de avaliaçoes da funcao objetivo = "<<arc_global.contQuantAvaliacoes<<endl;;

	// list<SolucaoEdgeSet *> lisd = arc_global.getElementos();
	// list<SolucaoEdgeSet *>::iterator i = lisd.begin();
	// 	SolucaoEdgeSet *s;
	// 	while (i != lisd.end()) {
	// 		s = *i;
	// 		fprintf(stdout,"%.10lf %.10lf\n",s->getObj(0),s->getObj(1));
	// 		i++;
	// 	}
	
	//arc_global.printSetPoints(stdout);
	fprintf(stdout,"Tempo(s) Final = %.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
	fprintf(tempofile,"%.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );

	// cout<<(double) contSucessPrimTrans/contPrimTrans<<endl;;
	// cout<<(double) contSucessBT/contBT<<endl;;

	arc_global.printSetPoints(stdout);
	arc_global.printSetPoints(samplefile);
	fprintf(samplefile,"\n");

	fclose(samplefile);
	fclose(tempofile);

}


int main(int argc, char *argv[]){
	int seemente = std::atoi(argv[1]);
	AVALICACOES = std::atoi(argv[2]);
	init_genrand64(seemente);
	cout<<"========= Estatisticas ========= "<<endl;
	cout<<"Semente utilizada : "<<seemente<<endl;
	input(); // ler instância
	cout<<"Instância lida..."<<endl;

	samplefile = fopen(argv[3],"a");
	tempofile = fopen(argv[4],"a");




	times(&tempoAntes);
		
	transgenetic();

	times(&tempoDepois);
	cout<<"Quantidade de geracoes = "<<contGeracoes<<endl;
	cout<<"Quantidade de renovacoes = "<<contRenovacoes<<endl;
	// cout<<"Quantidade de plasm1 definido a cada renovacao = ";
	// for (int i=0; i<contRenovacoes+1; i++) cout<<quantPlas1_evolucao[i]<<",";
	// cout<<endl;
	// cout<<"Quantidade de plasm2 definido a cada renovacao = ";
	// for (int i=0; i<contRenovacoes+1; i++) cout<<quantPlas2_evolucao[i]<<",";
	// cout<<endl;
	// cout<<"Quantidade de plasm3 definido a cada renovacao = ";
	// for (int i=0; i<contRenovacoes+1; i++) cout<<quantPlas3_evolucao[i]<<",";
	cout<<endl;
	cout<<"Quantidade total de ataque dos plasm1 = "<<contPlasm1<<endl;
	cout<<"Quantidade total de sucesso do plasm1 = "<<contSucessoPlasm1<<endl;
	cout<<"Média de melhoria do plasm1 para o obj1 = "<<(contPlasm1==0?0:(float)sumMelhoriaPlasm1_obj1/contPlasm1)<<endl;;
	cout<<"Média de melhoria do plasm1 para o obj2 = "<<(contPlasm1==0?0:(float)sumMelhoriaPlasm1_obj2/contPlasm1)<<endl;;

	cout<<"Quantidade total de ataque dos plasm2 = "<<contPlasm2<<endl;
	cout<<"Quantidade total de sucesso do plasm2 = "<<contSucessoPlasm2<<endl;
	cout<<"Média de melhoria do plasm2 para o obj1 = "<<(contPlasm2==0?0:(float)sumMelhoriaPlasm2_obj1/contPlasm2)<<endl;;
	cout<<"Média de melhoria do plasm2 para o obj2 = "<<(contPlasm2==0?0:(float)sumMelhoriaPlasm2_obj2/contPlasm2)<<endl;;

	cout<<"Quantidade total de ataque dos plasm3 = "<<contPlasm3<<endl;
	cout<<"Quantidade total de sucesso do plasm3 = "<<contSucessoPlasm3<<endl;
	cout<<"Média de melhoria do plasm3 para o obj1 = "<<(contPlasm3==0?0:(float)sumMelhoriaPlasm3_obj1/contPlasm3)<<endl;;
	cout<<"Média de melhoria do plasm3 para o obj2 = "<<(contPlasm3==0?0:(float)sumMelhoriaPlasm3_obj2/contPlasm3)<<endl;;


	cout<<"Quantidade total de ataque dos primTrans = "<<contPrimTrans<<endl;
	cout<<"Quantidade total de sucesso do primTrans = "<<contSucessPrimTrans<<endl;
	cout<<"Média de melhoria do primTrans para o obj1 = "<<(contPrimTrans==0?0:(float)sumMelhoriaPrimTrans_obj1/contPrimTrans)<<endl;;
	cout<<"Média de melhoria do primTrans para o obj2 = "<<(contPrimTrans==0?0:(float)sumMelhoriaPrimTrans_obj2/contPrimTrans)<<endl;;

	cout<<"Quantidade total de invocacao da BT = "<<contBT<<endl;
	cout<<"Quantidade total de sucesso da BT = "<<contSucessBT<<endl;
	cout<<"Média de melhoria da BT para o obj1 = "<<(contBT==0?0:(float)sumMelhoriaBT_obj1/contBT)<<endl;;
	cout<<"Média de melhoria da BT para o obj2 = "<<(contBT==0?0:(float)sumMelhoriaBT_obj2/contBT)<<endl;;

	cout<<"Quantidade de avaliaçoes da funcao objetivo = "<<arc_global.contQuantAvaliacoes<<endl;;

	// list<SolucaoEdgeSet *> lisd = arc_global.getElementos();
	// list<SolucaoEdgeSet *>::iterator i = lisd.begin();
	// 	SolucaoEdgeSet *s;
	// 	while (i != lisd.end()) {
	// 		s = *i;
	// 		fprintf(stdout,"%.10lf %.10lf\n",s->getObj(0),s->getObj(1));
	// 		i++;
	// 	}
	
	//arc_global.printSetPoints(stdout);
	fprintf(stdout,"Tempo(s) Final = %.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
	fprintf(tempofile,"%.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );

	// cout<<(double) contSucessPrimTrans/contPrimTrans<<endl;;
	// cout<<(double) contSucessBT/contBT<<endl;;

	arc_global.printSetPoints(stdout);
	arc_global.printSetPoints(samplefile);
	fprintf(samplefile,"\n");

	fclose(samplefile);
	fclose(tempofile);

}