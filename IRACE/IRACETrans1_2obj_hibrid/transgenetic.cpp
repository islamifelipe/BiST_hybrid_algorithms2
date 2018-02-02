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
#include <fstream>      // std::ifstream 



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

// quantPlas1, quantPlas2 e quantPlas3 sao utilizados pelo procedimento criarPlasmideos 
// e sao autilizados a cada ini_ger_sett geraçoes consecutivas
int quantPlas1; // quantidade (probabilidade de escolha) do plasmideo 1
int quantPlas2; // quantidade (probabilidade de escolha) do plasmideo 2
int quantPlas3; // quantidade (probabilidade de escolha) do plasmideo 3

int contPlasm1 = 0; // conta quantas vezes o plasmideo rmc-prim tenta atacar
int contSucessoPlasm1 = 0; // conta quantos sucessos o plasm1 teve em atacar
float sumMelhoriaPlasm1 = 0; // conta o quanto o plasm1 melhora
int contPlasm2 = 0; // conta quantas vezes o plasmideo "de uma soluçao" tenta atacar
int contSucessoPlasm2= 0; // conta quantos sucessos o plasm2 teve em atacar
float sumMelhoriaPlasm2= 0; // conta o quanto o plasm2 melhora
int contPlasm3= 0; // conta quantas vezes o plasmideo "de duas soluçoes" tenta atacar
int contSucessoPlasm3= 0; // conta quantos sucessos o plasm3 teve em atacar
float sumMelhoriaPlasm3= 0; // conta o quanto o plasm3 melhora
int contPrimTrans= 0; // conta a quantidade e vezes em que o primTrans tenta atacar
int contSucessPrimTrans= 0; // conta a quantidade de sucesso que o primTrans obteve
float sumMelhoriaPrimTrans= 0; // conta o quanto o PrimTrans melhora
int contBT = 0;
int contSucessBT = 0;
float sumMelhoriaBT = 0;

int amostralARESTAS[NUMEROVERTICES*(NUMEROVERTICES-1)/2][2];

//parametros proIrace
int numgeracoes;
int numplasmideos1;
int ini_ger_sett;
float ppprob_vector;
float ppprob_factor;
int max_tabuu; 
int tabutenuree;
int max_vizinhoss;

void input(const char *instanceName){
	int n; // esta leitura de "n" é somente para cumprir o formato da instância. Os valores de fato estao em param.h
	std::ifstream inn;
    inn.open(instanceName, std::ifstream::in);
	inn>>n; // quantidade de vertices
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
			inn>>org;
			inn>>dest;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				inn>>custos[ob][i][j];
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

void criaPlasmideos(Plasmideo *pls){
	int tam, cont=0, index2, index3;
	for (int i=0; i<quantPlas1 && cont<numplasmideos1; i++){
		tam = IRandom(2,(int)(0.5*(NUMEROVERTICES-1)));// NOTAR: DIFERENTE DE SILVIA
		double randd = genrand64_real3();
		double lambda[2] = {randd, 1.0 - randd};
		pls[cont].geraPlasm_rmcPrim(lambda, tam);
		pls[cont].tipo = 1;
		cont++;
	}

	for (int i=0; i<quantPlas2 && cont<numplasmideos1; i++){
		index2 = IRandom(0,arc_global.getSize()-1);
		tam = IRandom(2,(int)(0.5*(NUMEROVERTICES-1)));// NOTAR: DIFERENTE DE SILVIA
		pls[cont].geraPlasm_Solucao(*arc_global.getSolucao(index2), tam);
		pls[cont].tipo = 2;
		cont++;
	}

	for (int i=0; i<quantPlas3 && cont<numplasmideos1; i++){
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

void computeContSucss(int numPlas){ // computa quantos ataques foram bem sucedidos para cada palsmideo
	if (numPlas == 1){
		contSucessoPlasm1++;
	} else if (numPlas == 2){
		contSucessoPlasm2++;
	} else if (numPlas == 3){ // 
		contSucessoPlasm3++;
	}
}

void crowndDistance(){ // by Felipe
		#define INF 1e9
		ParetoSet nds;
		list<SolucaoEdgeSet *> lixeira1 = arc_global.lixeira;
		list<SolucaoEdgeSet *>::iterator it = lixeira1.begin();
		// cout<<"Tamanho da lixeira ANTES = "<<lixeira1.size()<<endl;
		while (it!=lixeira1.end()){
			nds.adicionarSol((*it));
			it++;
		}
		//list<SolucaoEdgeSet *> arqq  = arc_global.getElementos();
		//lixeira.splice(lixeira.end(), arqq); // une a lixeira ao arquivo externo,
		list<SolucaoEdgeSet *> lixeira = nds.getElementos();
		double randd = genrand64_real3();
		double lambda[2] = {randd, 1.0 - randd};
		int l = lixeira.size();
		// cout<<"Tamanho da lixeira = "<<l<<endl;
		it = lixeira.begin();
		while (it!=lixeira.end()){
			if (genrand64_real3() < 0.5) {
				contBT++;
				if (buscaTabu((*it), lambda)) contSucessBT++;
				 // experimentos preliminares comprovaram que aqui é melhor
				/*Justificativa da probabilidade de 50% : com esta probabiliade, é possível mesclar 
				soluçoes intensificadas (melhores) e soluçoes nao-intensificadas*/
			}
			(*it)->distance = 0;
			it++;
		}
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
		#undef INF
	}

void transgenetic(){
	double prob_vectorr = ppprob_vector;
	alocaPopulacao(populacao);
	gerarPopulacao1(populacao);
	quantPlas1 = round(numplasmideos1/3); // valor inicial
	quantPlas2 = round(numplasmideos1/3); // valor inicial
	quantPlas3 = round(numplasmideos1/3); // valor inicial
	PrimTransposon ptrans;
	int conttt = 0;
	for (int i=0; i<numgeracoes; i++){  // para cada geraçao
		Plasmideo *pls = new Plasmideo[numplasmideos1];
		criaPlasmideos(pls); /// cria plasmideos
		// cout<<"Geracao "<<i+1<<endl;
		for (int pppt = 0; pppt < TAMANHOPOPULACAO; pppt++){ // cada individuo sofre um ataque de plasmideo ou de transpon

			if(genrand64_real3() < prob_vectorr){ // um plasmideo ataca
				// escolhe-se um plasmideo
				int indexPlas = IRandom(0,numplasmideos1-1); // escolhe o plasmideo
				computeContPlas(pls[indexPlas].tipo); // incrementa a quantidade de escolha deste palsmideo
				SolucaoEdgeSet *copia = new SolucaoEdgeSet(NUMEROVERTICES-1);
				*copia = *populacao[pppt];
				pls[indexPlas].atacaSolucao(*copia);
				bool resul = arc_global.adicionarSol(copia);
				if (*copia >> *populacao[pppt] || resul == true){ // critério de aceitaçao
					*populacao[pppt] = *copia;
					computeContSucss(pls[indexPlas].tipo); // computa aqui contSucessoPlasm1, contSucessoPlasm2 e contSucessoPlasm3
				}
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
					*populacao[pppt] = *copia;
					contSucessPrimTrans++;
				} 
			}
		}

		conttt++;
		if (conttt == ini_ger_sett){
			// redefine as quantidade individuais de cada plasmideo e cada transponsson
			// incrementa a probabilidade (PROB_VECTOR) de escolha do grupo de vetor que deve atacar
			// recicla a populaçao
			quantPlas1 = (int) round((float)contSucessoPlasm1/(contSucessoPlasm1 + contSucessoPlasm2 + contSucessoPlasm3)*numplasmideos1);
			quantPlas2 = (int) round((float)contSucessoPlasm2/(contSucessoPlasm1 + contSucessoPlasm2 + contSucessoPlasm3)*numplasmideos1);
			quantPlas3 = numplasmideos1 - quantPlas1 - quantPlas2;
			
			// cout<<"quantPlas1 = "<<quantPlas1<<" quantPlas2 = "<<quantPlas2<<" quantPlas3 = "<<quantPlas3<<endl;
			// cout<<"contSucessoPlasm1 = "<<contSucessoPlasm1<<" contSucessoPlasm2 = "<<contSucessoPlasm2<<" contSucessoPlasm3 = "<<contSucessoPlasm3<<endl;
			// contSucessoPlasm1 = 0; contSucessoPlasm2 = 0; contSucessoPlasm3 =0
			// // quantPlas1, quantPlas2, quantPlas3 serao utilizados em criarPlasmideos
			prob_vectorr += ppprob_factor;
			crowndDistance();
			conttt = 0;
			
		}
	}
}

int main(int argc, char *argv[]){

	char *instanceName = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-numgeracoes") == 0) {
            numgeracoes = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-numplasmideos1") == 0) {
            numplasmideos1 = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-ini_ger_sett") == 0) {
            ini_ger_sett = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-ppprob_vector") == 0) {
            ppprob_vector = atof(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-ppprob_factor") == 0) {
            ppprob_factor = atof(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-max_tabuu") == 0) {
            max_tabuu = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-tabutenuree") == 0) {
            tabutenuree = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-max_vizinhoss") == 0) {
            max_vizinhoss = atoi(argv[i+1]);
            i++;
        }

        if (strcmp(argv[i], "-instance") == 0 || strcmp(argv[i], "-i") == 0) {
            instanceName = argv[i+1];
            i++;
        }
    }


   	int semente = time(NULL);
	init_genrand64(semente);

	input(instanceName); // ler instância
	


	transgenetic();

	// ostringstream convert;   // stream used for the conversion
	std::ofstream outt;
	// convert << semente;      // insert the textual representation of 'Number' in the characters in the stream

	// string arqui = "Pareto_"+string(convert);
	string namePareto  = "pareto"+std::to_string(semente)+"_"+std::to_string(NUMEROVERTICES)+".txt";
    string nameHyp = "hyp"+std::to_string(semente)+"_"+std::to_string(NUMEROVERTICES)+".txt";
    outt.open(namePareto, std::ofstream::out);
	list<SolucaoEdgeSet *> lisd = arc_global.getElementos();
	list<SolucaoEdgeSet *>::iterator i = lisd.begin();
	SolucaoEdgeSet *s;
	while (i != lisd.end()) {
		s = *i;
		//fprintf(outt,"%.10lf %.10lf\n",s->getObj(0),s->getObj(1));
		outt<<s->getObj(0)<<" "<<s->getObj(1)<<endl;
		i++;
	}
	string cmd = "./hyp_ind hyp_ind_param_Fev.txt "+ namePareto +" refereceFiltredE3.out "+ nameHyp;
	system(cmd.c_str());
	std::ifstream innn;
    innn.open(nameHyp, std::ifstream::in);
    double hypervolume;
	innn>>hypervolume; // hypervolume;
	fprintf(stdout,"%.9e\n",hypervolume);
	return 0;

}