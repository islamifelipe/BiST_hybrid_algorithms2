#ifndef ZG_H
#define ZG_H

#include "rand64/mersenne64.c"

extern int numgeracoes;
extern int numplasmideos1;
extern int ini_ger_sett;
extern float ppprob_vector;
extern float ppprob_factor;
extern int max_tabuu; 
extern int tabutenuree;
extern int max_vizinhoss;


#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // qualquer coisa menor que esse valor, é considerado 0
#define PROFUNDIDADEGRID 5 

#define NUMOBJETIVOS 2 // FIXO
#define NUMEROVERTICES 100 // Varia //IRACE INSTANCIA 
#define NUMEROARESTAS (NUMEROVERTICES-1)//IRACE INSTANCIA 

#define TAMANHOPOPULACAO 100  // tamanho da populaçao
//IRACE #define QUANTGERACOES 30  i(25-45)//quantidade de iteracoes do TANSGENETICO (IRACE)
//IRACE #define NUMPLASMIDEOS 15 c(15,18,21,24) // QUANTIDADE DE PLASMIDEOS (IRACE)
/*Há 3 tipos de plasmideos. A quantidade de plasmideos criada para cada tipo depende da taxa de eficiência do mesmo.
 Inicialmente,  todos tem a mesma quantidade (5) (probabiliade 33,333%)*/

//IRACE #define INI_GER_SET 8 i(4,8) //6 //4 // a cada INI_GER_SET geraçoes consecutivas, PROB_VECTOR é incrementado, as probilidades de cada plasmideo e cada transponsson sao atualizadas indivualmente, e a populaçao é reciclada
//IRACE #define PROB_VECTOR 0.5 r(0.4,0.65) // probabilidade de executar os plasmideos (1 - PROB_VECTOR é a probabiliade de executar os transponssons)
//IRACE #define PROB_FACTOR 0.1 r(0.1, 0.16) // fator que incrementa a probilidade dos plasmideos e decrementa a probabilidade dos transpossons
//PROB_VECTOR + PROB_FACTOR


//IRACE #define MAX_TABU 10 i(10, 18)// quantidade maxima de iteracoes da busca tabu
//IRACE #define TABUTENURE 5 i(4,6)// quantidade de iteracoes em que uma aresta permanece como Tabu
//IRACE #define MAX_VIZINHOS 15 i(10,17)// numero maximo de vizinhos

#endif
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
#define NUMEROVERTICES 500
#define NUMEROVERTICES 600
#define NUMEROVERTICES 700
#define NUMEROVERTICES 800
#define NUMEROVERTICES 900
#define NUMEROVERTICES 1000
