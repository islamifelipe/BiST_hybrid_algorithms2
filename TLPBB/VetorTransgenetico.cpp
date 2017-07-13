#ifndef VETORTRANSGENETICO_CPP
#define VETORTRANSGENETICO_CPP

#include <cassert>
#include "SolucaoEdgeSet.cpp"
#include "param.h"
#include "ParetoSet.cpp"
#include <list>
#include <string>

#define FOR(i,a,b) for(int i=a;i<b;i++)
class VetorTransgenetico {
	protected:
		TRandomMersenne *rg;
		int arestaEscolhida[2], tamTrecho;

		// TODAS AS FUNCOES "geraTrechoXXXX" deixam as "tamTrecho" primeiras arestas selecionadas
		void geraTrechoAleatorio(SolucaoEdgeSet *s) {
			int c = 0;
			bool naarvore[NUMEROVERTICES] = {  false };
			assert(s->confereArestas());

			//fprintf(stderr,"Inicio da selecao das arestas!\n");
			// descobre qual a aresta que vai comecar o trecho
			naarvore[arestaEscolhida[0]] = naarvore[arestaEscolhida[1]] = true;
			FOR(i,0,NUMEROVERTICES-1) {
				if (arestaEscolhida[0] == s->edges[i][0] && arestaEscolhida[1] == s->edges[i][1]) {
					std::swap( s->edges[i][0], s->edges[c][0] );
					std::swap( s->edges[i][1], s->edges[c][1] );
					c++;
					break;
				}
			}

			// cria uma lista das arestas conectadas a arvore atual
			int lista[NUMEROVERTICES], tamLista = 0;
			while (c < tamTrecho) {
				tamLista = 0;
				FOR(i,c,NUMEROVERTICES-1) {
					if (naarvore[s->edges[i][0]] || naarvore[s->edges[i][1]]) {
						//fprintf(stderr,"Colocando (%d,%d) na lista\n",s->edges[i][0],s->edges[i][1]);
						lista[tamLista++] = i;
					}
				}

				// a lista nunca pode estar vazia!
				if (!(tamLista > 0)) {
					fprintf (stderr, "TamLista: %d\n", tamLista);
				}
				assert(tamLista > 0);

				// escolhe aleatoriamente uma das arestas da lista
				int esc = lista[rg->IRandom(0,tamLista-1)];
				naarvore[s->edges[esc][0]] = naarvore[s->edges[esc][1]] = true;
				//fprintf(stderr,"Escolhida aresta (%d,%d)\n",s->edges[esc][0],s->edges[esc][1]);

				// joga a aresta "esc" para o comeco do conjunto
				std::swap( s->edges[esc][0], s->edges[c][0] );
				std::swap( s->edges[esc][1], s->edges[c][1] );
				c++;
			}

			// neste ponto, as arestas selecionadas sao as "tamTrecho" primeiras
			//fprintf(stderr,"Arestas escolhidas:\n");
			//FOR(i,0,tamTrecho) fprintf(stderr,"Aresta (%d,%d)\n",s->edges[i][0],s->edges[i][1]);
			//fprintf(stderr,"Fim da selecao das arestas!\n");
		}
};

#endif
