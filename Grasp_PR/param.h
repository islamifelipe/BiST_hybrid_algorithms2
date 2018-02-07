#ifndef ZG_H
#define ZG_H


#include "rand64/mersenne64.c"

// Parâmetros fornecidos pelo IRACE


extern int countAcceptConstrutivo;
extern int countAcceptBL;
extern int countAcceptPR;
extern int quantConstrutivo;
extern int quantBL;
extern int quantPR;



#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // quanquer coisa menor que esse valor, é considerado 0

#define NUMOBJETIVOS 2 
#define NUMEROVERTICES 20 // Varia
#define NUMEROARESTAS (NUMEROVERTICES-1)

// Parâmetros do arquivo externo
#define PROFUNDIDADEGRID 5 // profundidade da grid
#define MAXARCSIZE 300 // limite do arquivo externo


// parâmetro do GRASP
#define ALFA 0.08 // alfa para saber o tamanho da LRC
#define NUMVETORES_GRASP 700 //700 // numero de vetores de escalaizaçao para o grasp
#define INTERVALO_PR 49

//parâmetros da busca local
#define MAX_LS 30 // quantidade de iteraçoes da busca local
#define MAX_VIZ 10	// quantidade de vizinhos da busca local

#define maxPR 7

#endif

