#ifndef TRANSPONSON_CPP
#define TRANSPONSON_CPP

#include <cassert>
#include "SolucaoEdgeSet.cpp"
#include "param.h"
#include "ParetoSet.cpp"
#include "VetorTransgenetico.cpp"
#include "auxEdgeStruct.h"

bool fitVecNodeComp (fitVecNode a, fitVecNode b) {
	if (a.c < b.c) return true;
	return false;
}

class Transposon {
	private:
		TRandomMersenne *rg;
		SolucaoEdgeSet *s [NUMSOLTRANSPOSON];
		ParetoSet local;
		fitVecNode * fitVec[NUMESCVECTORS];

	public:
		Transposon(TRandomMersenne &r) {
			for (int i = 0; i < NUMSOLTRANSPOSON; i++)
				s[i] = new SolucaoEdgeSet (NUMEROVERTICES-1, r);
			rg = &r;
			local.clear();

			double alfa[NUMESCVECTORS][2];
			alfa[0][0] = 0.0;
			alfa[0][1] = 1.0-alfa[0][0];
			alfa[1][0] = 0.25;
			alfa[1][1] = 1.0-alfa[1][0];
			alfa[2][0] = 0.5;
			alfa[2][1] = 1.0-alfa[2][0];
			alfa[3][0] = 0.75;
			alfa[3][1] = 1.0-alfa[3][0];
			alfa[4][0] = 1.0;
			alfa[4][1] = 1.0-alfa[4][0];
			for (int i = 5; i < NUMESCVECTORS; i++) {
				alfa[i][0] = rg->Random();
				alfa[i][1] = 1.0 - alfa[i][0];
			}

			int numare = NUMEROVERTICES*(NUMEROVERTICES-1)/2;

			for (int k = 0; k < NUMESCVECTORS; k++)
				fitVec [k] = new fitVecNode[numare];

			for (int k = 0; k < NUMESCVECTORS; k++) {
				int ctrvar = 0;
				for (int i = 0; i < NUMEROVERTICES; i++) {
					for (int j = i+1; j < NUMEROVERTICES; j++) {
						fitVec [k][ctrvar].a = i;
						fitVec [k][ctrvar].b = j;
						fitVec [k][ctrvar].c = alfa[k][0]*f(0,i,j) + alfa[k][1]*f(1,i,j);
						ctrvar++;
					}
				}

				stable_sort ( fitVec[k], fitVec[k]+NUMEROVERTICES*(NUMEROVERTICES-1)/2, fitVecNodeComp );
			}

		}

		~Transposon() {

			for (int i=0;i<NUMSOLTRANSPOSON;i++)
				delete s[i];

			for (int i = 0; i < NUMESCVECTORS; i++)
				delete fitVec[i];
		}

		//gera NUMSOLTRANSPOSON solucoes
		//tenta adicionar em um arquivo local, que contem a solucao que sera alterada
		//se solucoes nao dominadas forem geradas, uma eh escolhida aleatoriamente e substituir a sol do parametro
		bool executar (SolucaoEdgeSet &ind, int index) {

			bool resp [NUMSOLTRANSPOSON];
			int aux [NUMSOLTRANSPOSON];
			int quant = 0;
			local.adicionarSol (&ind);

			for (int i = 0; i < NUMSOLTRANSPOSON; i++) {
				*s[i] = ind;
			}
			int are = rg->IRandom (0, NUMEROARESTAS-1);
			for (int i = 0; i < NUMSOLTRANSPOSON; i++) {
				s[i]->substAresta ((are+i)%NUMEROARESTAS, index, fitVec);
				resp[i] = local.adicionarSol (s[i]);
				if (resp[i]) {
					aux[quant] = i;
					quant++;
				}
				else resp[i] = false;
			}
			if (quant > 0) {
				ind = *s[ aux[rg->IRandom (0, quant-1)] ];
				return true;
			}
			else
				return false;

		}

		bool executar2 (SolucaoEdgeSet &ind, int index) {

			bool resp [NUMSOLTRANSPOSON];
			int aux [NUMSOLTRANSPOSON];
			int quant = 0;
			local.adicionarSol (&ind);

			for (int i = 0; i < NUMSOLTRANSPOSON; i++) {
				*s[i] = ind;
			}
			int are = rg->IRandom (0, NUMEROARESTAS-1);
			for (int i = 0; i < NUMSOLTRANSPOSON; i++) {
				s[i]->substAresta ((are+i)%NUMEROARESTAS, index, fitVec);
				resp[i] = local.adicionarSol (s[i]);
				if (resp[i]) {
					aux[quant] = i;
					quant++;
					for (int j = i+1; j < NUMSOLTRANSPOSON; j++)
						*s[j] = *s[i];
				}
				else resp[i] = false;
			}
			if (quant > 0) {
				ind = *s[ aux[rg->IRandom (0, quant-1)] ];
				return true;
			}
			else
				return false;
		}

		void executar3 (SolucaoEdgeSet &ind, int iindex) {
		}

};

#endif
