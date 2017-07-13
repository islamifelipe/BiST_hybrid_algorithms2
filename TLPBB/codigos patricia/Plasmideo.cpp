#ifndef PLASMIDEO_CPP
#define PLASMIDEO_CPP

#include <cassert>
#include "SolucaoEdgeSet.cpp"
#include "param.h"
#include "ParetoSet.cpp"
#include "rmcPrim.cpp"
#include <list>
#include <string>
#include <iostream>
using namespace std;

#define FOR(i,a,b) for(int i=a;i<b;i++)
class Plasmideo {
	private:
	int trechoInserir[NUMEROARESTAS][2];
	UnionFind uf;
    TRandomMersenne *rg;
    int tamTrecho;

	public:
	Plasmideo(){}
	Plasmideo(TRandomMersenne *r) {
		rg = r;
	}
	void setRandom(TRandomMersenne *r) { rg = r; }

    void printTrechoInserir () {
        FOR(i, 0, tamTrecho) {
            FOR(j, 0, 2) {
                cout << trechoInserir [i][j] << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }

	void geraPlasm_rmcPrim (double *lambda, int tam) {
        tamTrecho = tam;
		SolucaoEdgeSet agm(NUMEROVERTICES,*rg);
		rmcPrim (agm,lambda,*rg, tamTrecho);

        FOR(i, 0, tamTrecho) {
            trechoInserir [i][0] = agm.edges[i][0];
            trechoInserir [i][1] = agm.edges[i][1];
        }
	}

	void geraPlasm_Solucao(SolucaoEdgeSet &s, int tam) {

		tamTrecho = tam;

        int c = 0;
        bool naarvore[NUMEROVERTICES] = {  false };
		
        int e = rg->IRandom(0,NUMEROARESTAS-1);
		
        trechoInserir[0][0] = s.edges[e][0];
		trechoInserir[0][1] = s.edges[e][1];
		
        naarvore[s.edges[e][0]] = naarvore[s.edges[e][1]] = true;
        
        std::swap( s.edges[e][0], s.edges[c][0] );
        std::swap( s.edges[e][1], s.edges[c][1] );
        c++;
        
        // cria uma lista das arestas conectadas a arvore atual
        int lista[NUMEROVERTICES], tamLista = 0;
        while (c < tamTrecho) {
            tamLista = 0;
            FOR(i,c,NUMEROVERTICES-1) {
                if (naarvore[s.edges[i][0]] || naarvore[s.edges[i][1]]) {
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
            naarvore[s.edges[esc][0]] = naarvore[s.edges[esc][1]] = true;
            //fprintf(stderr,"Escolhida aresta (%d,%d)\n",s->edges[esc][0],s->edges[esc][1]);

            // joga a aresta "esc" para o comeco do conjunto
            std::swap( s.edges[esc][0], s.edges[c][0] );
            std::swap( s.edges[esc][1], s.edges[c][1] );
            c++;
        }

        FOR(i, 1, tam) {
            trechoInserir [i][0] = s.edges[i][0];
            trechoInserir [i][1] = s.edges[i][1];
        }

	}
	
	/* Funcionamento: coloca as arestas do trecho do plasmideo, tenta colocar o maximo possivel de arestas
		originais da solucao e caso a solucao nao fique completa, preenche com arestas quaisquer */
	void atacaSolucao(SolucaoEdgeSet &s) {
		uf.clear();
		SolucaoEdgeSet nova = s;
		int contEdges = 0;

		// coloca as arestas do trecho selecionado
		FOR(i,0,tamTrecho) {
			nova.edges[contEdges][0] = trechoInserir[i][0];
			nova.edges[contEdges++][1] = trechoInserir[i][1];
			uf.unionClass(trechoInserir[i][0],trechoInserir[i][1]);
		}

		// tenta colocar as arestas da solucao
		FOR(i,0,NUMEROARESTAS) {
			if (!uf.sameClass(s.edges[i][0],s.edges[i][1])) {
				nova.edges[contEdges][0] = s.edges[i][0];
				nova.edges[contEdges++][1] = s.edges[i][1];
				uf.unionClass(s.edges[i][0],s.edges[i][1]);
			}
		}

		// completa a solucao com arestas quaisquer
		while (contEdges < NUMEROARESTAS) {
			int e[2] = { rg->IRandom(0,NUMEROVERTICES-1), rg->IRandom(0,NUMEROVERTICES-1) };
			if (uf.sameClass(e[0],e[1])) continue;
			nova.edges[contEdges][0] = e[0];
			nova.edges[contEdges++][1] = e[1];
			uf.unionClass(e[0],e[1]);
		}
		nova.calcularFitness();
		s = nova;
	}

};

#endif
