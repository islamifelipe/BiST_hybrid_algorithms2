#ifndef PRIMTRANSPOSON
#define PRIMTRANSPOSON

#include "param.h"
#include "SolucaoEdgeSet.cpp"
#include "auxEdgeStruct.h"

#include <list>
#include <string>
#include <math.h>
#include <ctime>

#include <iostream>

using namespace std;

#define FOR(i,a,b) for(int i=a;i<b;i++)

//extern int contPrimTrans; // conta a quantidade e vezes em que o primTrans tenta atacar


/*This code file was kindly provided by Monteiro */




class PrimTransposon{
	public:
		PrimTransposon(){}
		
		/*Permite que um número aleatório entre 5 e 10% das arestas de s permaneçam na solução, completando com arestas geradas a partir do rmcprim */
		void atacaSolucao(SolucaoEdgeSet &s, double lambda[NUMOBJETIVOS]) { // altera soluçao "s"
			//contPrimTrans++;
            int reftam; // quantidade de arestas que poderao permanecer na solucao "s"
			int nArestas = NUMEROVERTICES-1;
            if (NUMEROVERTICES < 30)
                reftam = IRandom (1, 4); // DEIXEI ISSO AQUI PORQUE PODE SER QUE QUEIRAMOS TESTAR COM MENOS QUE 30 VERTICES (MAS INICIALMENTE NAO!)
            else
                reftam = IRandom((int)(nArestas*0.05), (int)(nArestas*0.1));
			cout<<"reftam = "<<reftam<<endl;
			auxEdgeSet fitVec [nArestas];
			
			for (int i = 0; i < nArestas; i++) {
				fitVec[i].fit = 0.0;
				for (int objk = 0; objk<NUMOBJETIVOS; objk++)
					fitVec[i].fit += lambda[objk]*f(objk, s.edges[i][0], s.edges[i][1]);
				fitVec[i].a = s.edges[i][0];
				fitVec[i].b = s.edges[i][1];
			}

			stable_sort (fitVec, fitVec + nArestas, auxEdgeSetComp);

			/*soteiam-se aleatoriamente reftam arestas diferentes*/
			/*uma aresta é escolhida se o rand for maior que seu prob[i] e menor ou igual ao prob de i+1*/
			/*estas arestas vao para ref*/
			SolucaoEdgeSet ref (reftam);

			int tam = 0;
			double prob;
			int resp [reftam];
			bool isNew;
			while (tam != reftam) { // escolher as arestas que ficarão

				prob = sin (genrand64_real3()*PI/(-2.0)) + 1.0;
				//assert (!(prob < 0) && !(prob > 1));

				int ind = (int)(prob*((double)(nArestas))); //gera o indice do vetor de fitness. 
				if (ind == nArestas)
					ind = (int)( (prob/2.0) * ((double)(nArestas)) ); //gera o indice do vetor de fitness. 
				
				//assert (ind > -1 && ind < nArestas);

				isNew = true;
				for (int bla = 0; (bla < tam) && isNew; bla++) { // se a aresta ainda nao foi inserida na soluçao corrente
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
			mcprim (ref, reftam, s, lambda);
			//clock_t finish = clock();
			//fprintf (stderr, "\ntempo mcprim: %f\n", ((double)(finish-start))/CLOCKS_PER_SEC);
		}
};

#endif