#ifndef ZG_H
#define ZG_H

#include "rand64/mersenne64.c"


#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // qualquer coisa menor que esse valor, é considerado 0
#define PROFUNDIDADEGRID 5 

#define NUMOBJETIVOS 2 // FIXO
#define NUMEROVERTICES 100 // Varia
#define NUMEROARESTAS (NUMEROVERTICES-1)

#define TAMANHOPOPULACAO 100  // tamanho da populaçao
#define QUANTGERACOES 50  //quantidade de iteracoes do TANSGENETICO (IRACE)
#define QUANT_PLAS 10 // QUANTIDADE DE PLASMIDEOS (IRACE)
/*Há 3 tipos de plasmideos. A quantidade de plasmideos criada para cada tipo depende da taxa de eficiência do mesmo.
 Inicialmente,  todos tem a mesma quantidade (probabiliade 33,333%)*/

#endif
