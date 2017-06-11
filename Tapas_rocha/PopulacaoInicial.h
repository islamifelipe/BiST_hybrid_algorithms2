#ifndef _POPULACAOINICIAL_H_
#define _POPULACAOINICIAL_H_
#include <utility>      // std::pair
#include "Grafo.h"
#include "Arquivo.h"
#include <list>
/* pair<float, Aresta**> presort é uma pre-ordenaçao das arestas de acordo com o lambda (float) entre 0 e 1
*/
using namespace std;
pair<int*, pair<float, float> > rmcKruskal(Grafo *g,float lambda1, float lambda2,Aresta** presort, float num,  int quantObrigatorias);
vector< pair<int*, pair<float, float> > > getPopulacaoInicial(Grafo *g, Arquivo &global_arc,  int quantObrigatorias);
pair<int*, pair<float, float> > RandomWalk(Grafo *g, int quantObrigatorias);
pair<int*, pair<float, float> > clone(Grafo *g,pair<int*, pair<float, float> > &s);

bool thereis(vector< pair<int*, pair<float, float> > > &vetor, pair<int*, pair<float, float> > &tree );

//vector< pair<int*, pair<float, float> > > getPopulacaoInicialTapas(Grafo *g, Arquivo &global_arc);

//void diversify(Grafo *g, vector< pair<int*, pair<float, float> > > &pop);

#endif