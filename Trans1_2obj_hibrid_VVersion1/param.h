#ifndef ZG_H
#define ZG_H

#include "rand64/mersenne64.c"

// PARÂMETROS DO IRACE

#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // qualquer coisa menor que esse valor, é considerado 0
#define PROFUNDIDADEGRID 5 

#define NUMOBJETIVOS 2 // FIXO
#define NUMEROVERTICES 600 // Varia
#define NUMEROARESTAS (NUMEROVERTICES-1)

#define TAMANHOPOPULACAO 100  // tamanho da populaçao #popSize
#define QUANTGERACOES 40  //quantidade de iteracoes do TANSGENETICO (IRACE)
#define NUMPLASMIDEOS 15 //18 //15 // QUANTIDADE DE PLASMIDEOS (IRACE)
/*Há 3 tipos de plasmideos. A quantidade de plasmideos criada para cada tipo depende da taxa de eficiência do mesmo.
 Inicialmente,  todos tem a mesma quantidade (5) (probabiliade 33,333%)*/

#define INI_GER_SET 8 //8 //6 //4 // a cada INI_GER_SET geraçoes consecutivas, PROB_VECTOR é incrementado, as probilidades de cada plasmideo e cada transponsson sao atualizadas indivualmente, e a populaçao é reciclada
#define PROB_VECTOR 0.41 // 0.65 //0.6 //0.5 // probabilidade de executar os plasmideos (1 - PROB_VECTOR é a probabiliade de executar os transponssons)
#define PROB_FACTOR 0.11 //0.12//0.16 //0.1 // fator que incrementa a probilidade dos plasmideos e decrementa a probabilidade dos transpossons
//PROB_VECTOR + PROB_FACTOR


#define MAX_TABU 13 //15 // 10 // quantidade maxima de iteracoes da busca tabu
#define TABUTENURE 5 //4 // 5 // quantidade de iteracoes em que uma aresta permanece como Tabu
#define MAX_VIZINHOS 16 // 10 // 15 // numero maximo de vizinhos

#endif


/**
	#popSize
	#max\_gen
	#numPlasTotal = #plas1 + #plas2 + #plas3
	#intGerSet
	#probPlasm
	#probFator
	#maxTabu
	#tabutenure
**/
#define NUMEROVERTICES 50
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 100
#define NUMEROVERTICES 100
#define NUMEROVERTICES 100
#define NUMEROVERTICES 1000
