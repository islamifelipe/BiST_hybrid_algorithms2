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
#define MAXARCSIZE 300 // limite do arquivo externo //#max_sol


// parâmetro do GRASP
#define ALFA 0.08 // alfa para saber o tamanho da LRC //parâmetro BETA
#define NUMVETORES_GRASP 700 //#numVetores // numero de vetores de escalaizaçao para o grasp
#define INTERVALO_PR 49 // #intervaloPR

//parâmetros da busca local
#define MAX_LS 10 // quantidade de iteraçoes da busca local \\ #max_bl
#define MAX_VIZ 30	// quantidade de vizinhos da busca local \\ #maxViz

#define maxPR 7 // #max_pr

#endif

