#ifndef _PARAMETROS_H_
#define _PARAMETROS_H_

#define MAXARCSIZE 300 // tamanho maximo do arquivo global (este valor é fixo)

#define por_max_rmckrus 70 // ****porcentagem**** maxima (baseados no tamanho da populacao) de cromossomos retornados pelo rmcKruskal (usado para compor a populacao inicial)
#define por_max_suportadas 20 //****porcentagem**** maxima de solucoes suportadas para a populacao inicial
#define size_pop 150 // tamanho da populacao
#define max_gen 20 // 25 // Quantidade maxima de geracoes 
#define max_offsp 10 // quantidade maxima de tentativas para gerar algum descendente valido por geracao
//#define max_it 5 // quantidade de geraçoes consecutivas que o Global_Arc fica sem mudar 
#define alfaMutacao 0.03 // probabilidade de mutacao no individuo
#define max_neighbors 5 //6 // numero maximo de vizinhos
#define max_tabu 30//15 // 14? 20 // quantidade maxima de iteracoes da busca tabu
#define tabutenure 5//3 // quantidade de iteracoes em que uma aresta permanece como Tabu
#define maxPR 7//15 // quantidade maxima de iteracoes do PR
// #define maxSolucoes 3000 // 3000 ?? 10000 // quantidade maxima de solucoes que serao armazenadas no arquivo externo (max_sol)
// #define maxParetoAproximativo 300 // 400 ou 300? 500 // quantidade maxima de pontos do pareto aproximativo submetidos ao path relinking (max_apr)
#define maxVizinhos 400//100
//#define max_it 3


#endif