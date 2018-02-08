#ifndef _PARAMETROS_H_
#define _PARAMETROS_H_


extern int max_gen;// 20 - 25 // Quantidade maxima de geracoes 
extern int max_offsp; // 5 - 10 quantidade maxima de tentativas para gerar algum descendente valido por geracao
extern float alfaMutacao; //0.03 - 0.1 probabilidade de mutacao no individuo
extern int max_neighbors; //5-9 // numero maximo de vizinhos
extern int max_tabu; //15-30 // quantidade maxima de iteracoes da busca tabu
extern int tabutenure; //3-5 // quantidade de iteracoes em que uma aresta permanece como Tabu
extern int maxPR; //7 - 15 // quantidade maxima de iteracoes do PR
extern int maxVizinhos;// 60 - 100

#define MAXARCSIZE 300 // tamanho maximo do arquivo global (este valor é fixo)

#define por_max_rmckrus 70 // ****porcentagem**** maxima (baseados no tamanho da populacao) de cromossomos retornados pelo rmcKruskal (usado para compor a populacao inicial)
#define por_max_suportadas 20 //****porcentagem**** maxima de solucoes suportadas para a populacao inicial
#define size_pop 150 // tamanho da populacao

// IRACE #define max_gen 20 // 20 - 25 // Quantidade maxima de geracoes 
// IRACE #define max_offsp 10 // 5 - 10 quantidade maxima de tentativas para gerar algum descendente valido por geracao
// IRACE #define alfaMutacao 0.03 //0.03 - 0.1 probabilidade de mutacao no individuo
// IRACE #define max_neighbors 5 //5-9 // numero maximo de vizinhos
// IRACE #define max_tabu 30//15-30 // quantidade maxima de iteracoes da busca tabu
// IRACE #define tabutenure 5//3-5 // quantidade de iteracoes em que uma aresta permanece como Tabu
// IRACE #define maxPR 7//7 - 15 // quantidade maxima de iteracoes do PR
// IRACE #define maxVizinhos 70 // 60 - 100



#endif