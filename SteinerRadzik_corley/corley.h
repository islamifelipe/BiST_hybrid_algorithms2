#ifndef _CORLEY_H_
#define _CORLEY_H_

#include <utility>      // std::pair
#include <list>
#include "Grafo.h"

/*
# This code implements the Corley (1985) algoritm
*/

/*quantVertices deve ser a quantidade de vertices pertencentes ao state
O state terá quantVertices-1 arestas */
list <pair<int*,  pair<float, float> > > corley(Grafo *g, pair<int*, pair<float, float> > state,  int quantVertices);


#endif