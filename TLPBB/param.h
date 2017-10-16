#ifndef ZG_H
#define ZG_H

#include "rand/randomc.h"
#include <limits.h>

/* OPCOES DE DEBUG */
//#define DEBUG
#ifdef DEBUG
#define D(x) x
#endif
#ifndef DEBUG
#define D(x)
#endif

//#define ASSERT
#ifdef ASSERT
#define ASS(x) x
#endif
#ifndef ASSERT
#define ASS(x)
#endif

#define id(i,j) idArestas[i][j]
#define f(k,i,j) custos[k][i][j] // k-ésimo custo da aresta de (i,j)
#define isObrigatoria1(i,j) isObrigatoria[i][j] // se true, estao a aresta de i,j é obrigatoria
#define EPS 1e-9

#define NUMOBJETIVOS 2

#define NUMTRANSPONSONS 2

#define NUMESCVECTORS 10
#define NUMPLASMIDEOS 10
#define MAXIMOFALHASBUSCA 2

#define NUMEROVERTICES 600
#define NUMSOLTRANSPOSON 3
//#define NUMSOLTRANSPOSON (int)(NUMEROVERTICES*0.05)
#define NUMEROARESTAS (NUMEROVERTICES-1)
#define PROFUNDIDADEGRID 5
#define TAMANHOPOPULACAO 150 /* FIXME 200 */
#define NUMEROITERACOES 30 /* FIXME 500 */
#define NUMEROMCPRIM 142   /* NUMERO DE SOLUCOES INICIAIS GERADAS PELO MCPRIM */
#define ALGORITMOCONVERGIU 10 /* NUMERO DE GERACOES SEM MUDANCA NO PARETO SET PARA DIVERSIFICAR */
#define PCTDIVERSIFICACAO 0.0 /* PARCELA DA POPULACAO A SER ALTERADA NA DIVERSIFICACAO */

#endif
#define NUMEROVERTICES 50
// #define NUMEROVERTICES 100
// #define NUMEROVERTICES 200
// #define NUMEROVERTICES 300
// #define NUMEROVERTICES 400
// #define NUMEROVERTICES 500
// #define NUMEROVERTICES 600
// #define NUMEROVERTICES 700
// #define NUMEROVERTICES 800
// #define NUMEROVERTICES 900
// #define NUMEROVERTICES 1000
#define NIL INT_MAX
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 400
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 400
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 400
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 400
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 400
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
#define NUMEROVERTICES 200
#define NUMEROVERTICES 300
#define NUMEROVERTICES 400
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
#define NUMEROVERTICES 50
