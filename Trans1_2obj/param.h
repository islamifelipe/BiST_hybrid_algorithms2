#ifndef ZG_H
#define ZG_H

#include "rand64/mersenne64.c"


#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // qualquer coisa menor que esse valor, é considerado 0
#define PROFUNDIDADEGRID 5 

#define NUMOBJETIVOS 2 // FIXO
#define NUMEROVERTICES 300 // Varia
#define NUMEROARESTAS (NUMEROVERTICES-1)

#define TAMANHOPOPULACAO 100  // tamanho da populaçao
#define QUANTGERACOES 30  //quantidade de iteracoes do TANSGENETICO (IRACE)
#define NUMPLASMIDEOS 15 // QUANTIDADE DE PLASMIDEOS (IRACE)
/*Há 3 tipos de plasmideos. A quantidade de plasmideos criada para cada tipo depende da taxa de eficiência do mesmo.
 Inicialmente,  todos tem a mesma quantidade (5) (probabiliade 33,333%)*/

#define INI_GER_SET 8 //4 // a cada INI_GER_SET geraçoes consecutivas, PROB_VECTOR é incrementado, as probilidades de cada plasmideo e cada transponsson sao atualizadas indivualmente, e a populaçao é reciclada
#define PROB_VECTOR 0.5 // probabilidade de executar os plasmideos (1 - PROB_VECTOR é a probabiliade de executar os transponssons)
#define PROB_FACTOR 0.1 // fator que incrementa a probilidade dos plasmideos e decrementa a probabilidade dos transpossons
//PROB_VECTOR + PROB_FACTOR

#endif
