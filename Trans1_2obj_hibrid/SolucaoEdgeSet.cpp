#ifndef SOLUCAOZG_CPP
#define SOLUCAOZG_CPP

/*This code file was kindly provided by Monteiro */


#include <cstdio>
#include <queue>
#include "Solucao.cpp"
#include "UnionFind.cpp"
#include "param.h"
#include "auxEdgeStruct.h"
#include <algorithm>
#include <iostream>

using namespace std;

extern double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];

typedef struct {
	int listaadj[NUMEROVERTICES][NUMEROVERTICES], graus[NUMEROVERTICES];
	bool completo;
} grafo;



class SolucaoEdgeSet : public Solucao {
	public:
	int edges[NUMEROVERTICES-1][2];
	int nEdges;
	UnionFind uf;
	grafo *g;
	double distance; // crownd distance // utilizada no NSGA-II e na reciclagem do Hudson
	double antigof[NUMOBJETIVOS];

	SolucaoEdgeSet(int n) {
		nEdges = n;
		f[0] = f[1] = 0.0;
		g = NULL;
	}
	~SolucaoEdgeSet() {

	}

    // copia somente os valores de fitness
    // util para guardar copias de solucoes somente para comparacao
    void shallowCopy(SolucaoEdgeSet &s) {
        s.f[0] = f[0];
        s.f[1] = f[1];
    }

	/* Calcula o fitness atual da solucao
	 * Complexidade O(N) */
	void calcularFitness() {
		f[0] = f[1] = 0.0;
		for (int i=0;i<NUMEROVERTICES-1;i++)
			for (int j=0;j<2;j++)
				f[j] += f(j,edges[i][0],edges[i][1]);

		// cout<<f[0]<<" "<<f[1]<<endl;
	}

	/* Calcula o fitness de uma soluçao parcial com tam arestas
	 * Complexidade O(N) */
	void calcularFitness(int tam) {
		f[0] = f[1] = 0.0;
		for (int i=0;i<tam;i++)
			for (int j=0;j<2;j++)
				f[j] += f(j,edges[i][0],edges[i][1]);

		// cout<<f[0]<<" "<<f[1]<<endl;
	}

	/* gera uma arvore aleatoria
	 * Complexidade n */
    void doRandomWalk() {

        int vertlist [NUMEROVERTICES];
        for (int i = 0; i < NUMEROVERTICES; i++)
            vertlist[i]=i;
        int vertctr = NUMEROVERTICES-1; //tamanho valido da lista;

        int v_ind = IRandom(0,vertctr);
        int v_esc = vertlist[v_ind];
        vertlist [v_ind] = vertlist[vertctr];
        vertlist [vertctr] = v_esc;
        vertctr--;
        int viz1, viz2, viz2_ind;

        int cont = 0;
        while (cont < NUMEROVERTICES-1) {

            viz1 = vertlist [IRandom(vertctr+1, NUMEROVERTICES-1)];
            viz2_ind = IRandom(0,vertctr);
            viz2 = vertlist[viz2_ind];

            vertlist [viz2_ind] = vertlist[vertctr];
            vertlist [vertctr] = viz2;
            vertctr--;

            edges[cont][0] = viz1;
            edges[cont][1] = viz2;
            cont++;
        }
        calcularFitness();
       // assert (confereArestas());
    }



	/* Gera um individuo aleatorio
	 * Complexidade O(n lg n) */
	void geraIndividuoAleatorio() {
	    g = new grafo;
		g->completo = true;

		for (int i=0;i<NUMEROVERTICES;i++)
				g->graus[i] = NUMEROVERTICES;
		doRandomWalk();
		delete g;
	}

	/* Faz a troca das arestas ai e aj, religando no formato 2-OPT
	 * Complexidade O(1) */
	void trocaArestas(int ai, int aj, char tipo, SolucaoEdgeSet &soloriginal) {
		int a = soloriginal.edges[ai][0];
		int b = soloriginal.edges[ai][1];
		int c = soloriginal.edges[aj][0];
		int d = soloriginal.edges[aj][1];

		//inicialmente temos a-b e c-d

		int novaA[2]={0,0}, novaB[2]={0,0};
		novaA[0] = a;
		novaB[0] = b;

		// se todos ficarem na mesma componente, junta AC/BD, se nao junta AD/BC
		if (tipo == 0) {
			// faz a uniao AC e BD
			novaA[1] = c;
			novaB[1] = d;
		} else if (tipo == 1){
			// faz a uniao AD e BC
			novaA[1] = d;
			novaB[1] = c;
		}
		if (novaA[0]<novaA[1]){
			edges[ai][0] = novaA[0];
			edges[ai][1] = novaA[1];
		} else {
			edges[ai][0] = novaA[1];
			edges[ai][1] = novaA[0];
		}
		if (novaB[0]<novaB[1]){
			edges[aj][0] = novaB[0];
			edges[aj][1] = novaB[1];
		} else {
			edges[aj][0] = novaB[1];
			edges[aj][1] = novaB[0];
		}
	}

	char calcularTrocaArestas(int ai, int aj, SolucaoEdgeSet &soloriginal) {
		// Calcula a possivel troca das arestas "ai" com "aj"
		// Complexidade O(n)
		// uniao busca
		uf.clear();
		for (int k=0;k<NUMOBJETIVOS;k++) f[k] = 0.0; // (re)inicializa os objetivos
		
		for (int i=0;i<NUMEROVERTICES-1;i++)
			if (i != ai && i != aj) {
				edges[i][0] = soloriginal.edges[i][0];
				edges[i][1] = soloriginal.edges[i][1];
				uf.unionClass(edges[i][0],edges[i][1]);
				for (int k=0;k<NUMOBJETIVOS;k++)
					f[k] +=  f(k,edges[i][0],edges[i][1]);
			}
		int a = soloriginal.edges[ai][0];
		int b = soloriginal.edges[ai][1];
		int c = soloriginal.edges[aj][0];
		int d = soloriginal.edges[aj][1];

		char tipoTroca;
		// junta a-c e b-d
		uf.unionClass(a,c);
		uf.unionClass(b,d);
		// se todos ficarem na mesma componente, junta AC/BD, se nao junta AD/BC
		if (uf.sameClass(a,b) && uf.sameClass(a,c) && uf.sameClass(a,d)) {
			// faz a uniao AC e BD
			tipoTroca = 0;
			for (int k=0;k<NUMOBJETIVOS;k++)
				f[k] = f[k] + ( f(k,a,c) + f(k,b,d) );
		} else {
			// faz a uniao AD e BC
			tipoTroca = 1;
			for (int k=0;k<NUMOBJETIVOS;k++)
				f[k] = f[k] + ( f(k,a,d) + f(k,b,c) );
		}
		return tipoTroca;
	}

	void mutacao(SolucaoEdgeSet &sol){
		int a1 = IRandom(0,NUMEROVERTICES-1-1), a2;
		while ((a2 = IRandom(0,NUMEROVERTICES-1-1)) == a1);
		trocaArestas(a1,a2,calcularTrocaArestas(a1,a2,sol),sol);
	}

	//efetua remocao de aresta ai, adicionando a aresta de menor custo que reconecta a arvore
	void substAresta (int ai, int index, fitVecNode * fitVec[]) {


		uf.clear();

		antigof[0] = getObj(0);
		antigof[1] = getObj(1);
		f[0] = f[1] = 0.0;

		int a = edges[ai][0];
		int b = edges[ai][1];
		for (int i=0;i<NUMEROVERTICES-1;i++) {
			if (i != ai) {
				uf.unionClass(edges[i][0],edges[i][1]);
				for (int k=0;k<NUMOBJETIVOS;k++)
					f[k] +=  f(k,edges[i][0],edges[i][1]);
			}
		}

		int edge = 0;
		// anda ate a proxima aresta que pode ser inserida

		while (uf.sameClass(fitVec[index][edge].a,fitVec[index][edge].b)) edge++;

		if ( (fitVec[index][edge].a == a && fitVec[index][edge].b == b) ||
				(fitVec[index][edge].a == b && fitVec[index][edge].b == a) ) {
			edge++;
			while (uf.sameClass(fitVec[index][edge].a,fitVec[index][edge].b)) edge++;
		}
		// coloca a aresta na solucao
		edges[ai][0] = fitVec[index][edge].a;
		edges[ai][1] = fitVec[index][edge].b;
		setObj(0,getObj(0)+f(0,edges[ai][0],edges[ai][1]));
		setObj(1,getObj(1)+f(1,edges[ai][0],edges[ai][1]));
		uf.unionClass( fitVec[index][edge].a, fitVec[index][edge].b );

	//	assert (confereArestas());
	}

	void desfazerCalculoTroca() {
		f[0] = antigof[0];
		f[1] = antigof[1];
	}

	// TEM DE SER CHAMADO ANTES DA TROCA!
	int getArestaEntrou(int i, int j, char tipo, int arest, int no) {
		if (tipo == 0) {
			int nos[2][2] = { {edges[i][0],edges[j][0]}, {edges[i][1],edges[j][1]} };
			return nos[ arest ][ no ];
		} else {
			int nos[2][2] = { {edges[i][0],edges[j][1]}, {edges[i][1],edges[j][0]} };
			return nos[ arest ][ no ];
		}
	}
	int getArestaSaiu(int i, int j, char tipo, int arest, int no) {
		int nos[2][2] = { {edges[i][0],edges[i][1]}, {edges[j][0],edges[j][1]} };
		return nos[ arest ][ no ];
	}



	void operator=(SolucaoEdgeSet &s) {
		nEdges = s.nEdges;
		f[0] = s.f[0];
		f[1] = s.f[1];
		memcpy(edges,s.edges,sizeof(edges));
		antigof[0] = s.antigof[0];
		antigof[1] = s.antigof[1];
	}

	void printSolucao(FILE *f) {
		fprintf(f,"Custos = (%.3lf,%.3lf)\n",this->f[0],this->f[1]);
		fprintf(f,"Arestas = \n");
		for (int i=0;i<NUMEROVERTICES-1;i++)
			fprintf(f,"(%d - %d)\n",edges[i][0],edges[i][1]);
		fprintf(f,"\n");
	}

	void printPonto(FILE *f) {
		fprintf(f,"%.6lf %.6lf\n",getObj(0),getObj(1));
	}

	bool confereArestas() {
		uf.clear();
		bool usados[NUMEROVERTICES] = {false};
		int numUsados = 0;
		for (int i=0;i<NUMEROVERTICES-1;i++) {
			if (uf.sameClass(edges[i][0],edges[i][1])) return false;
			uf.unionClass(edges[i][0],edges[i][1]);
			for (int j=0;j<2;j++) {
		                if (!usados[edges[i][j]]) {
	               			usados[edges[i][j]] = true;
        			        numUsados++;
                		}
			}
		}
		if (numUsados != NUMEROVERTICES) return false;
		return true;
	}

	bool confereObjetivos() {
		double v[2] = {0.0,0.0};
		for (int i=0;i<NUMEROVERTICES-1;i++) {
			v[0] += f(0,edges[i][0],edges[i][1]);
			v[1] += f(1,edges[i][0],edges[i][1]);
		}
		if (fabs(v[0])-EPS > f[0] || fabs(v[1])-EPS > f[1]) return false;
		return true;
	}

	/*Sorteia-se uma aresta e pra sair. Sorteia-se uma aresta e' que pode entrar tal que:
		e' nao é dominada por e
		e' nao forma ciclo

	@param intervalo (0, iI), 0<=iI<NUMVERTICES-1 que diz a faixa de sorteio da aresta a ser excluída
	// TODO: receber um array de arestas pre-ordenado de acordo com um vetor de escalalizaçao
	*/
	int getVizinho1(int iI, SolucaoEdgeSet &soloriginal, double lambda[NUMOBJETIVOS]){
		uf.clear();
		for (int k=0;k<NUMOBJETIVOS;k++) f[k] = 0.0; // (re)inicializa os objetivos
		
		int e = IRandom(0,iI);
		for (int i=0;i<NUMEROVERTICES-1;i++){
			if (i != e) {
				edges[i][0] = soloriginal.edges[i][0];
				edges[i][1] = soloriginal.edges[i][1];
				uf.unionClass(edges[i][0],edges[i][1]);
				for (int k=0;k<NUMOBJETIVOS;k++)
					f[k] +=  f(k,edges[i][0],edges[i][1]);
			}
		}

		int amostral[NUMEROVERTICES*NUMEROVERTICES][2];
		int conttAmotral = 0; 
		// ATENCAO: GRAFO COMPLETO
		double obj0 = f(0, soloriginal.edges[e][0], soloriginal.edges[e][1]);
		double obj1 = f(1, soloriginal.edges[e][0], soloriginal.edges[e][1]);
		for (int i=0;i<NUMEROVERTICES-1;i++) {
			for (int j=i+1;j<NUMEROVERTICES;j++) {
				//if ((obj0<=f(0,i,j) && obj1<=f(1,i,j) && (obj0<f(0,i,j) || obj1<f(1,i,j)))==false){
					if ((f(0,i,j)*lambda[0] + f(1,i,j)*lambda[1] < obj0*lambda[0] + obj1*lambda[1]) && uf.sameClass(i,j)==false){
						amostral[conttAmotral][0] = i;
						amostral[conttAmotral][1] = j;
						conttAmotral++;
					}
				//}
			}
		}

		//cout<<"conttAmotral = "<<conttAmotral<<endl;
		if (conttAmotral>0){
			int index = IRandom(0, conttAmotral-1);
			edges[e][0] = amostral[index][0];
			edges[e][1] = amostral[index][1];
		} else { // nao foi possivel encontrar um vizinho com os critérios pre-definidos
			edges[e][0] = soloriginal.edges[e][0];
			edges[e][1] = soloriginal.edges[e][1];
		}
		for (int k=0;k<NUMOBJETIVOS;k++)
			f[k] += f(k,edges[e][0],edges[e][1]);
		return e;
	}

	bool isTree(){ // verificador
		//cout<<"Teste  = ";
		UnionFind uf;
		bool vetkktk[NUMEROVERTICES];
		double obj0 = 0, obj1 = 0;
		for(int i=0; i<NUMEROVERTICES; i++)vetkktk[i] = false;
		for (int i=0; i<NUMEROVERTICES-1; i++){
			if (uf.sameClass(this->edges[i][0],this->edges[i][1])==false){
				obj0 += f(0, this->edges[i][0], this->edges[i][1]);
				obj1 += f(1, this->edges[i][0], this->edges[i][1]);
				vetkktk[this->edges[i][0]] = true;
				vetkktk[this->edges[i][1]] = true;
				uf.unionClass(this->edges[i][0],this->edges[i][1]);
			} else {
				cout<<"ERROROROROROROROROROROROROROR TREEEE 1 "<<endl;
				return false;
			}
		}
		for(int i=0; i<NUMEROVERTICES; i++){
			if (vetkktk[i]==false) {
				cout<<"ERROROROROROROROROROROROROROR TREEEE 2"<<endl;
				return false;
			}
		}
		if (obj0!=this->getObj(0) || obj1!=this->getObj(1)){
			cout<<"ERROROROROROROROROROROROROROR OBJETIVOS 3"<<endl;
			return false;
		}
		cout<<"It's a tree! õ/"<<endl;;
		return true;
	}

};

#endif
