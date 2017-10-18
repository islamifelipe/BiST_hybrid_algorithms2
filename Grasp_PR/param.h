#ifndef ZG_H
#define ZG_H

/*This code file was kindly provided by Monteiro */

#include "rand/randomc.h"



#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // quanquer coisa menor que esse valor, é considerado 0

#define NUMOBJETIVOS 2 // Varia
#define NUMEROVERTICES 50 // Varia
#define NUMEROARESTAS (NUMEROVERTICES-1)

// Parâmetros do arquivo externo
#define PROFUNDIDADEGRID 5 // profundidade da grid
#define MAXARCSIZE 300 // limite do arquivo externo


// parâmetro do GRASP
#define ALFA 0.01
#define NUMVETORES_GRASP 700

//parâmetros da busca local
#define MAX_LS 30
#define MAX_VIZ 20

#endif
