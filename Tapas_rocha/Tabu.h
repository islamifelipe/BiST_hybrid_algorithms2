#ifndef _TABU_H_
#define _TABU_H_

#include <list>
#include <map>
#include "Grid.h"
#include <utility>      // std::pair
#include "Conjunto.h"
#include "Grafo.h"
#include "Arquivo.h"
#include "Parametros.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include "PopulacaoInicial.h"

using namespace std;

void buscaTabu(Grafo *g, pair<int*, pair<float, float> > &s, Arquivo *local_arc, Arquivo *global_arc);
bool m_grid(Arquivo *local_arc, pair<int*, pair<float, float> > sl, pair<int*, pair<float, float> > s);
bool belong(Arquivo *arq, pair<int*, pair<float, float> > s);
#endif