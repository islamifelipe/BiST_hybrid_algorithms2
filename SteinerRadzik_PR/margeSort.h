#ifndef _MARGESORT_H_
#define _MARGESORT_H_

#include "Aresta.h"
#define PRECISAO 0.00001

void intercala(float xl, float yl, float xll, float yll, int p, int q, int r, Aresta **v, int size, int direto);
void mergesort(float xl, float yl, float xll, float yll, Aresta **v, int size, int direto);



bool equalfloat(float a, float b);


bool maiorQuefloat(float a, float b);

bool maiorIgualQuefloat(float a, float b);

#endif