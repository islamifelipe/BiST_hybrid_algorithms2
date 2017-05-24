#ifndef _PARAMETROS_H_
#define _PARAMETROS_H_

#define por_max_rmckrus 40 // ****porcentagem**** maxima (baseados no tamanho da populacao) de cromossomos retornados pelo rmcKruskal (usado para compor a populacao inicial)
#define por_max_suportadas 50 //****porcentagem**** maxima de solucoes suportadas para a populacao inicial
#define size_pop 150 // tamanho da populacao
#define max_gen 25 // Quantidade maxima de geracoes 
#define max_offsp 5 // quantidade maxima de tentativas para gerar algum descendente valido por geracao
//#define max_it 5 // quantidade de geraçoes consecutivas que o Global_Arc fica sem mudar 
#define alfaMutacao 0.04 // probabilidade de mutacao no individuo
#define max_neighbors 5 // numero maximo de vizinhos
#define max_tabu 20 // quantidade maxima de iteracoes da busca tabu
#define tabutenure 3 // quantidade de iteracoes em que uma aresta permanece como Tabu
#define maxPR 6 // quantidade maxima de iteracoes do PR
#define maxSolucoes 10000 // quantidade maxima de solucoes que serao armazenadas no arquivo externo (max_sol)
#define maxParetoAproximativo 500 // quantidade maxima de pontos do pareto aproximativo submetidos ao path relinking (max_apr)
//#define max_it 3


#endif