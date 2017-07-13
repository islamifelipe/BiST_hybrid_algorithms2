#ifndef GERAPOP_CPP
#define GERAPOP_CPP

#include "param.h"
#include "SolucaoEdgeSet.cpp"
#include "LexKruskal.cpp"
#include "auxEdgeStruct.h"

#include <queue>
#include <algorithm>
#include <iostream>
using namespace std;


struct twoint {
	int a;
	int b;
};

//variaveis globais
auxEdgeStruct arestas [(NUMEROVERTICES*(NUMEROVERTICES-1))/2];
LexKruskal lkr;
int numIndGenerated = 0;
queue <struct twoint> fila;

//prototipos de funcoes
inline bool auxEdgeStructLexCompObj1 (auxEdgeStruct e1, auxEdgeStruct e2);
								//comparacao segundo criterio lexicografico que considera obj1 preponderante sobre obj2

inline bool auxEdgeStructLexCompObj2 (auxEdgeStruct e1, auxEdgeStruct e2);
								//comparacao segundo criterio lexicografico que considera obj2 preponderante sobre obj1
inline bool auxEdgeStructCompFit (auxEdgeStruct e1, auxEdgeStruct e2);
								//comparacao com base no fit

int geraPopSR1 (SolucaoEdgeSet pop [], double custos [NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES], int tampop);
								//procedimento principal para gerar a populacao com base na fase 1 do Steiner e Radzik

void borderSearch (SolucaoEdgeSet pop [], int tampop, double custos [NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES]);
								//procedimento auxiliar da fase 1 do Steiner e Radzik, chamado por geraPopSR1,
								//para gerar todos os individios da curva iterativamente com base no melhor
								//ponto da fronteira para o obj1 e para o obj2.



inline bool auxEdgeStructLexCompObj1 (auxEdgeStruct e1, auxEdgeStruct e2) {

	if (e1.c1 < e2.c1) return true;
	else if (e1.c1 == e2.c1 && e1.c2 < e2.c2) return true;
	return false;
}

inline bool auxEdgeStructLexCompObj2 (auxEdgeStruct e1, auxEdgeStruct e2) {

	if (e1.c2 < e2.c2) return true;
	else if (e1.c2 == e2.c2 && e1.c1 < e2.c1) return true;
	return false;
}

inline bool auxEdgeStructCompFit (auxEdgeStruct e1, auxEdgeStruct e2) {

	if (e1.fit < e2.fit) return true;
	return false;
}

int geraPopSR1 (SolucaoEdgeSet pop [], double custos [NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES], int tampop) {

	cout << "Entrei em GeraPopSR1" << endl;
	int arectr = 0;
	for (int i = 0; i < NUMEROVERTICES; i++) {
		for (int j = i+1; j < NUMEROVERTICES; j++) {
			arestas [arectr].a = i;
			arestas [arectr].b = j;
			arestas [arectr].c1 = custos [0][i][j];
			arestas [arectr].c2 = custos [1][i][j];
			arestas [arectr].fit = 0.0;
			arectr++;
		}
	}

	stable_sort (arestas, arestas + (NUMEROVERTICES*(NUMEROVERTICES-1))/2, auxEdgeStructLexCompObj1);
	lkr.executar (pop[0], arestas);
	numIndGenerated = 1;
	if (tampop > 1) {
		stable_sort (arestas, arestas + (NUMEROVERTICES*(NUMEROVERTICES-1))/2, auxEdgeStructLexCompObj2);
		lkr.executar (pop[1], arestas);
		if (pop[0] == pop[1]);
		else {
			numIndGenerated = 2;
			if (tampop > 2) {
				while (!fila.empty())
					fila.pop();
				twoint fstelement;
				fstelement.a = 0;
				fstelement.b = 1;
				fila.push(fstelement);
				borderSearch (pop, tampop, custos);
			}
		}
	}
	cout << "Saindo de GeraPopSR1" << endl;
	return numIndGenerated;

}

void borderSearch (SolucaoEdgeSet pop [], int tampop, double custos [NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES]) {

	cout << "Entrei em borderSearch" << endl;
	twoint atual;
	twoint prox;
	int i = numIndGenerated;
	cout << "generated itens: " << endl;
	while (i < tampop && !fila.empty()) {
		cout << "..." << endl;
		atual.a = fila.front().a;
		cout << "T1: fila.front().a == " << fila.front().a << endl;
		atual.b = fila.front().b;
		cout << "T2: fila.front().b == " << fila.front().b << endl;
		cout << "fila.size(): " << fila.size () << endl;
		if (fila.empty())
			cout << "empty" << endl;
		else
			cout << "not empty" << endl;
		fila.pop();
		cout << "T3" << endl;
		for (int j = 0; j < (NUMEROVERTICES*(NUMEROVERTICES-1))/2; j++) {
			//cout << "T4 -> j: " << j << endl;
			arestas[j].fit = arestas[j].c1*(pop[atual.a].getObj(1)-pop[atual.b].getObj(1)) +
					 arestas[j].c2*(pop[atual.b].getObj(0)-pop[atual.a].getObj(0));
			if (arestas[i].fit < 0.0) cout << j << ": NEGATIVO!!" << endl;
			//cout << "T5 -> j: " << j << endl;
			
		}
		cout << "T4" << endl;
		stable_sort (arestas, arestas + (NUMEROVERTICES*(NUMEROVERTICES-1))/2, auxEdgeStructCompFit);
		cout << "T5" << endl;
		lkr.executar (pop[i], arestas); //esta versao do kruskal considera que as arestas ja estao ordenadas
		cout << "T6" << endl;
		cout << i << endl;

		if (pop[i] == pop[0] || pop[i] == pop[1]);

		else {
			prox.a = atual.a;
			prox.b = i;
			fila.push (prox);
			prox.a = i;
			prox.b = atual.b;
			fila.push (prox);
			numIndGenerated += 1;
			i++;
		}
	}
	while (!fila.empty())
		fila.pop();

	cout << "saindo de borderSearch" << endl;
}

#endif
