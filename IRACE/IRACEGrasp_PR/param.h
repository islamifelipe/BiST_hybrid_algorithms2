#ifndef ZG_H
#define ZG_H


#include "rand64/mersenne64.c"


extern int numvetores_graaspp; //NUMVETORES_GRASP
extern int intervalo_prrr; //INTERVALO_PR
extern int maax_lss; //MAX_LS
extern int maxx_viz; //MAX_VIZ
extern int maxprr; //maxPR


#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // quanquer coisa menor que esse valor, é considerado 0

#define NUMOBJETIVOS 2 
#define NUMEROVERTICES 50 // Varia
#define NUMEROARESTAS (NUMEROVERTICES-1)

// Parâmetros do arquivo externo
#define PROFUNDIDADEGRID 5 // profundidade da grid
#define MAXARCSIZE 300 // limite do arquivo externo


// parâmetro do GRASP
#define ALFA 0.01 // alfa para saber o tamanho da LRC (FIXO)
// IRACE #define NUMVETORES_GRASP 700 //400--700 //700 // numero de vetores de escalaizaçao para o grasp
// IRACE #define INTERVALO_PR 55 //40--55

// //parâmetros da busca local
// IRACE #define MAX_LS 30//20--30 // quantidade de iteraçoes da busca local
//IRACE  #define MAX_VIZ 12//8-12 //10	// quantidade de vizinhos da busca local

// IRACE #define maxPR 10 // 6 - 10 //10 // maximo de iteraçoes do PR

#endif
#define NUMEROVERTICES 50
#define NUMEROVERTICES 100
