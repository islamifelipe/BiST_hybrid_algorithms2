#ifndef REC_PLASM_CPP
#define REC_PLASM_CPP

#include <cassert>
#include "SolucaoEdgeSet.cpp"
#include "param.h"
#include "ParetoSet.cpp"
#include "newmcprim.cpp"
#include "VetorTransgenetico.cpp"
#include <list>
#include <string>
#include <math.h>
#include <ctime>

#define PI 3.141592653589
#define FOR(i,a,b) for(int i=a;i<b;i++)

struct auxEdgeSet {

	int a;
	int b;
	double fit;

};


inline bool auxEdgeSetComp (auxEdgeSet e1, auxEdgeSet e2) {

	if (e1.fit < e2.fit) return true;
	return false;
}


class recPlasm : VetorTransgenetico {
	private:
		UnionFind uf;

	public:
		recPlasm(){}
		recPlasm(TRandomMersenne *r) {
			rg = r;
		}
		void setRandom(TRandomMersenne *r) { rg = r; }

		/*Funcionamento: permite que um número aleatório entre 5 e 10% das arestas de s permaneçam na solução, completando com arestas geradas a partir do rmckruskal */
		void atacaSolucao(SolucaoEdgeSet &s) {

			assert (s.confereArestas());
			double lambda [2];
			lambda [0] = rg->Random();
			lambda [1] = 1.0 - lambda[0];

            int reftam;
			int nArestas = NUMEROVERTICES-1;
            if (NUMEROVERTICES < 30)
                reftam = rg->IRandom (1, 4);
            else
                reftam = rg->IRandom((int)(nArestas*0.05), (int)(nArestas*0.1));
			assert (reftam > 0);


			auxEdgeSet fitVec [nArestas];
			
			for (int i = 0; i < nArestas; i++) {
				fitVec[i].fit = lambda[0]*f(0, s.edges[i][0], s.edges[i][1]) +
					        lambda[1]*f(1, s.edges[i][0], s.edges[i][1]);
				fitVec[i].a = s.edges[i][0];
				fitVec[i].b = s.edges[i][1];
			}

			stable_sort (fitVec, fitVec + nArestas, auxEdgeSetComp);

			/*soteiam-se aleatoriamente reftam arestas diferentes*/
			/*uma aresta é escolhida se o rand for maior que seu prob[i] e menor ou igual ao prob de i+1*/
			/*estas arestas vao para ref*/
			SolucaoEdgeSet ref (reftam, *rg);

			int tam = 0;
			double prob;
			int resp [reftam];
			bool isNew;
			while (tam != reftam) {

				prob = sin (rg->Random()*PI/(-2.0)) + 1.0;
				assert (!(prob < 0) && !(prob > 1));

				int ind = (int)(prob*((double)(nArestas))); //gera o indice do vetor de fitness. 
				if (ind == nArestas)
					ind = (int)( (prob/2.0) * ((double)(nArestas)) ); //gera o indice do vetor de fitness. 
				
				assert (ind > -1 && ind < nArestas);

				isNew = true;
				for (int bla = 0; (bla < tam) && isNew; bla++) {
					if (resp [bla] == ind)
						isNew = false;
				}
				if (isNew) {
					ref.edges[tam][0] = fitVec[ind].a;
					ref.edges[tam][1] = fitVec[ind].b;
					resp[tam] = ind;
					tam++;
				}
			}
			//clock_t start = clock();
			mcprim (ref, reftam, s, lambda, *rg);
			//clock_t finish = clock();
			//fprintf (stderr, "\ntempo mcprim: %f\n", ((double)(finish-start))/CLOCKS_PER_SEC);
		}
};

#endif
