#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sys/times.h>
#include <sys/types.h>
#include <iostream>
using namespace std;

#include "rmcKruskal.cpp"
#include "SolucaoEdgeSet.cpp"
#include "BoundedParetoSet.cpp"
#include "Plasmideo.cpp"
#include "recPlasm.cpp"
#include "rand/randomc.h"
#include "param.h"
#include "mcprim.cpp"
#include "Transposon.cpp"
#include "rmcPrim.cpp"

//variaveis globais
int numIndGenerated = 0;

BoundedParetoSet global;
SolucaoEdgeSet *pop[TAMANHOPOPULACAO];
//SolucaoEdgeSet *ja_inseridas[5];
double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
int numGeracoes = 0;
rmcKruskal kruskal;

int numFalhas[TAMANHOPOPULACAO];

void input(char *arq) {
	FILE *file = fopen(arq,"r");
	int k,l;
	int n; //by felipe NOT UTILIZED
	float peso1, peso2; //by FELIPE
	fscanf(file,"%d", &n);//by felipe;
	D(printf("1. Iniciando leitura dos custos\n");)
	for (int i=0;i<NUMEROVERTICES;i++) {
		for (int j=0;j<NUMEROVERTICES;j++) {
			custos[0][i][j] = 999999999999;// by FELIPE
			custos[1][i][j] = 999999999999;// by FELIPE
		}
	}
	while ( ! feof (file) ){
			//if (NUMEROVERTICES < 1000) // by FELIPE
			fscanf(file,"%d %d %f %f",&k,&l,&peso1, &peso2);
			//else fscanf(file,"%lf %lf",&custos[0][i][j],&custos[1][i][j]); // by FELIPE
			custos[0][l][k] = peso1;// by FELIPE
			custos[1][l][k] = peso2;// by FELIPE
			custos[0][k][l] = peso1;// by FELIPE
			custos[1][k][l] = peso2;// by FELIPE
			cout<<k<<" "<<l<<" "<<peso1<<" "<<peso2<<endl;// by FELIPE
		}
	//	}
	//}

	D(printf("1. Leitura dos custos\n"));

}


int main( int argc,  char *argv[]) {

	if (argc < 6) {
		printf("Arvore Geradora Minima Multi-Criterio - Algoritmo de Testes. Utilizacao: %s (arquivo da instancia) (semente aleatoria) (arquivo lixo global) (log pareto - by felipe) (log tempo - by felipe)\n\n\n",argv[0]);
		return 1;
	}


	TRandomMersenne rg( std::atoi(argv[2]) );
	input(argv[1]);

	global.setNomeGlobalf (argv[3]);

	FILE *paretoFront = fopen(argv[4],"a");
   	FILE *tempofile = fopen(argv[5],"a");

	/* CALCULA O TEMPO */
	struct tms tempoAntes, tempoDepois;
	times(&tempoAntes);

	/* Aloca a populacao */
	for (int i=0;i<TAMANHOPOPULACAO;i++) {
		pop[i] = new SolucaoEdgeSet(NUMEROVERTICES-1,rg);
		numFalhas[i] = 0;
	}
	//for (int i = 0; i < 5; i++)
	//	ja_inseridas[i] = new SolucaoEdgeSet (NUMEROVERTICES-1, rg);
	/* PASSO 1: gera pop inicial com rmcprim e randomwalk */

	/* gera as solucoes rmcPrim */
	int quant_rmcPrim = (int)(TAMANHOPOPULACAO*0.9);
	double fator = (quant_rmcPrim == 0 ? 0 : 1.0/quant_rmcPrim);
	double lambda [2] = {1.0, 0.0};
	int quant = 0, wctr = 0;
	int sucrmcPrim = 0;
	int rmcPrimbyrand = 0;
	int existectr = 0;
	int numIndGen = 0;



	while (quant < quant_rmcPrim && wctr < 1.5*quant_rmcPrim) {
		rmcPrim( *pop[numIndGen], lambda, rg );
		if (wctr != quant_rmcPrim) {
			lambda[0] -= fator;
			lambda[1] += fator;
		}
		else {
			lambda[0] = 1.0;
			lambda[1] = 0.0;
			fator = 2.0*fator;    
		}

		// impede que se coloquem solucoes repetidas
		bool existe = false;
		for (int j=0;j<numIndGen && !existe;j++) {
			if (*pop[numIndGen] == *pop[j]) {
				existe = true;
				existectr++;
			}
		}

		if (!existe) {
			bool resp = global.adicionarSol(pop[numIndGen]);
			if (resp) {
				sucrmcPrim++;
				numIndGen++;
				quant++;
			}
			else if (rg.Random() < 0.6) {
				rmcPrimbyrand++;
				numIndGen++;
				quant++;
			}
		}
		wctr++;
	}
	cerr << quant << " individuos gerados com o rmcPrim" << endl;
	cerr << '\t' << sucrmcPrim << " sucesso no rmcPrim" << endl;
	cerr << '\t' << rmcPrimbyrand << " rmcPrim no rand" << endl;
	cerr << '\t' << existectr << " individuos gerados que ja estavam na pop" << endl;


	int rwctr = 0;

	// gera o restante com randomwalk... 
	for (int i=numIndGen;i<TAMANHOPOPULACAO;i++)
		pop[i]->doRandomWalk();

	// adiciona todas ao arquivo global
	for (int i=numIndGen;i<TAMANHOPOPULACAO;i++) {
		if (global.adicionarSol(pop[i]))
			rwctr++;
	}

	cerr << "tamanho do arquivo global no inicio: " << global.getSize() << endl;
	//cout << rwctr << " individuos na fronteira de pareto com o random walk" << endl;


	// ALGORITMO TRANSGENETICO 

	recPlasm plasm (&rg);
	Transposon trans (rg);

	int plasm1suc = 0;
	int plasm1fail = 0;
	int totalplasm1 = 0;

	int plasm2suc = 0;
	int plasm2fail = 0;
	int totalplasm2 = 0;

	int transsuc = 0;
	int transfail = 0;
	int totaltrans = 0;

	int rectranssuc = 0;
	int rectransfail = 0;
	int totalrectrans = 0;

	double probPlasm = 0.5;
	double probTrans = 1.0 - probPlasm;
	double probFactor = 0.1;
	int changeProbItr = 4;

	// fatores de diversificacao quando o algoritmo convergir 
	int numSolucoes = global.getSize(), numRodadas = 0;
	do {

		if (numGeracoes%changeProbItr == 0) {
			probPlasm += probFactor;
			probTrans -= probFactor;
		}

		fprintf(stderr,"Geracao %d (%d solucoes)\n",numGeracoes,global.getSize());
		/*fprintf(stderr,"Grid:");
		  for (int i=0;i<(1<<(NUMOBJETIVOS*PROFUNDIDADEGRID));i++) fprintf(stderr," %d",global.getPositionCount(i));
		  fprintf(stderr,"\n");*/
		numGeracoes++;

		// PASSO 1: cria os vetores PLASMIDEOS
		Plasmideo pl[NUMPLASMIDEOS];
		for (int i=0;i<NUMPLASMIDEOS;i++)
			pl[i].setRandom(&rg);
		// Vetores 0 a 4: arvores geradoras minimas parametrizadas aleatoriamente
		for (int i=0;i<5;i++) {
			double l[2] = { rg.Random(), 0.0 };
			l[1] = 1.0-l[0];
			pl[i].geraPlasm_rmcPrim(l,rg.IRandom((int)(0.25*NUMEROARESTAS),(int)(0.5*NUMEROARESTAS)));
		}
		// Vetores 5 a 10: trechos de solucoes do pareto global que estao em locais pouco populados 

		//int quant_ins = 0;
		//bool ja_ins = false;
		int tam_ctr = 1;

		list<SolucaoEdgeSet *> solucoes = global.getElementos();
		for (int i=5;i<10;i++) {
			bool sair = false;
			for (int tam=tam_ctr;!sair;tam++) for(list<SolucaoEdgeSet *>::iterator it=solucoes.begin();it!=solucoes.end() && !sair;it++) {
				int c = global.getPositionCount( **it );
				if (c == tam) {

					pl[i].geraPlasm_Solucao(**it,rg.IRandom((int)(0.25*NUMEROARESTAS),(int)(0.5*NUMEROARESTAS)));
					tam_ctr = tam;
					sair = true;

					
					/*
					bool existe = false;
					if (ja_ins) {
						for (int j = 0; j < quant_ins; j++) {
							if (**it == *ja_inseridas[j]) {
								existe = true;
								break;
							}
						}
					}
					if (!existe) {
						ja_ins = true;
						*ja_inseridas[quant_ins] = **it;
						quant_ins++;
						pl[i].geraPlasm_Solucao(**it,rg.IRandom((int)(0.3*NUMEROARESTAS),(int)(0.6*NUMEROARESTAS)));
						tam_ctr = tam;
						sair = true;
					}
					*/
				}
			}
		}

		/*
		   cout << endl;
		   for (int i = 0; i < NUMPLASMIDEOS; i++) {
		   cout << "plasm [" << i << "]:" << endl;
		   pl[i].printTrechoInserir();
		   }
		   cout << endl;
		 */

		// TESTE TESTE TESTE TESTE TESTE

		for (int i=0;i<TAMANHOPOPULACAO;i++) {
			SolucaoEdgeSet nova = *pop[i];

			// Escolhe o tipo de Plasmideo que vai atacar a populacao
			int vetorPlasm = rg.IRandom(0,NUMPLASMIDEOS-1);
			double p = rg.Random ();

			if (p < probPlasm) {
				if (vetorPlasm < 5)
					totalplasm1++;
				else
					totalplasm2++;
				pl [vetorPlasm].atacaSolucao (nova);
				//assert (nova.confereArestas());
				if (nova >> *pop[i] || global.adicionarSol(&nova)) {
					//fprintf(stderr,"Solucao %d sendo atualizada\n",i);
					*pop[i]= nova;
					if (vetorPlasm < 5)
						plasm1suc++;
					else
						plasm2suc++;
				}
				else {
					if (vetorPlasm < 5)
						plasm1fail++;
					else
						plasm2fail++;
				}
			}

			else {
				if (rg.Random () < 0.3) {
					totaltrans++;
					trans.executar2(nova, rg.IRandom (0, NUMESCVECTORS-1));
					//assert (nova.confereArestas());

					if (nova >> *pop[i] || global.adicionarSol(&nova)) {
						*pop[i]= nova;
						//numFalhas[i] = 0;
						transsuc++;
					}
					else {
						transfail++;
						//numFalhas[i]++;
					}
				}
				else {
					totalrectrans++;
					nova = *pop[i];
					plasm.atacaSolucao (*pop[i]);
					//assert ( (*pop[i]).confereArestas() );
					//pl[ vetorPlasm ].atacaSolucao(*pop[i]);

					if (*pop[i] >> nova || global.adicionarSol(pop[i])) {rectranssuc++;}
					else {
						*pop[i] = nova;
						rectransfail++;
					}
					//numFalhas[i] = 0;
				}
			}
			//if (numFalhas[i] >= MAXIMOFALHASBUSCA) {
		}

		// Se necessario, divesifica a populacao
		if (global.getSize() == numSolucoes) {
			numRodadas++;
			// se o algoritmo convergiu
			if (numRodadas == ALGORITMOCONVERGIU) {
				// TODO o ideal era que a diversificacao fosse feita de diversas maneiras, talvez
				// gerando varias solucoes e escolhendo aquela que esta no ponto menos populoso
				for (int i=0;i<TAMANHOPOPULACAO*PCTDIVERSIFICACAO;i++) {
					pop[i]->geraIndividuoAleatorio();
				}
				numRodadas = 0;
				fprintf(stderr,"DIVERSIFICANDO %.1lf%% DA POPULACAO!\n",PCTDIVERSIFICACAO*100.0);
			}
		}
		else {
			numSolucoes = global.getSize();
			numRodadas = 1;
		}

		}
		while (numGeracoes < NUMEROITERACOES);
		

		// deleta os elementos da populacao e do offspring
		for (int i=0;i<TAMANHOPOPULACAO;i++) {
			delete pop[i];
		}
		//for (int i = 0; i < 5; i++)
		//	delete ja_inseridas[i];

		times(&tempoDepois);
		fprintf(stdout,"Tempo(s) = %.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
		fprintf(tempofile,"%.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
   	

		global.printSetPoints(stdout);
		global.printSetPoints(paretoFront);
		//FILE * sol = fopen ("solucoes.out", "w");
		//if (sol != NULL) { global.printAllSolutions (sol); fclose (sol);}

		global.clear();

		cerr << "plasm1suc: " << plasm1suc << endl;
		cerr << "plasm1fail: " << plasm1fail << endl;
		cerr << "totalplasm1: " << totalplasm1 << endl;
		cerr << "plasm1: percentual de melhoria -> " << (double)(plasm1suc)/(double)(totalplasm1) << endl << endl;

		cerr << "plasm2suc: " << plasm2suc << endl;
		cerr << "plasm2fail: " << plasm2fail << endl;
		cerr << "totalplasm2: " << totalplasm2 << endl;
		cerr << "plasm2: percentual de melhoria -> " << (double)(plasm2suc)/(double)(totalplasm2) << endl << endl;

		cerr << "transsuc: " << transsuc << endl;
		cerr << "transfail: " << transfail << endl;
		cerr << "totaltrans: " << totaltrans << endl;
		cerr << "trans: percentual de melhoria -> " << (double)(transsuc)/(double)(totaltrans) << endl << endl;

		cerr << "rectranssuc: " << rectranssuc << endl;
		cerr << "rectransfail: " << rectransfail << endl;
		cerr << "totalrectrans: " << totalrectrans << endl;
		cerr << "rectrans: percentual de melhoria -> " << (double)(rectranssuc)/(double)(totalrectrans) << endl;

		fprintf(paretoFront,"\n");
   		fclose(paretoFront);
		fclose(tempofile);

		return 0;
	}
