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
#include <math.h>

using namespace std;

extern double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];

typedef struct {
	int listaadj[NUMEROVERTICES][NUMEROVERTICES], graus[NUMEROVERTICES];
	bool completo;
} grafo;



class SolucaoEdgeSet : public Solucao {
	public:
	int pruffer[NUMEROVERTICES-2]; // cada index guarda um valor de 0 a NUMEROVERTICES-1
	int edges[NUMEROVERTICES-1][2];
	int nEdges;
	UnionFind uf;
	grafo *g;
	double distance; // crownd distance // utilizada no NSGA-II e na reciclagem do Hudson
	double antigof[NUMOBJETIVOS];
	int posicaoListaNSGAII; // guarda o index onde a soluçao é guardada na popupacao NUMPOPULACAO*2 do NSGA-II
	int prank; // diz em que front a solucao está
	bool isVisitada; // flag para o 
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

	// assume que o vetor "pruffer" nao está vazio.
	// esta sub-rotina deve ser invocada sempre que o "pruffer" for modificado
	// ja calcula o custo também
	void convertToTree(){
		f[0] = f[1] = 0.0;
		int degree[NUMEROVERTICES]; 
		for (int i=0; i<NUMEROVERTICES; i++){
			degree[i] = 1;
		}
		for (int i=0; i<NUMEROVERTICES-2; i++){
			degree[pruffer[i]] = degree[pruffer[i]] + 1;
		}
		int contArestasArvore = 0;
		int origem , destino ;
		for (int i=0; i<NUMEROVERTICES-2; i++){ // para cada rótulo no pruffer
			for (int j=0; j<NUMEROVERTICES; j++){ // para cada vértice
				if (degree[j]==1){
					origem = pruffer[i];
					destino = j;
					if (origem>destino){
						origem = j;
						destino =  pruffer[i];
					}
					edges[contArestasArvore][0] = origem;
					edges[contArestasArvore][1] = destino;
					f[0] += f(0,edges[contArestasArvore][0],edges[contArestasArvore][1]);
					f[1] += f(1,edges[contArestasArvore][0],edges[contArestasArvore][1]);
					contArestasArvore++;
					degree[pruffer[i]] = degree[pruffer[i]]-1;
					degree[j] = degree[j] - 1;
					break;
				}
			}
		}
		int u=-1,v=-1;
		for (int i=0; i<NUMEROVERTICES; i++){
			if (degree[i]==1){
				if(u==-1) u = i;
				else{
					v = i;
					break;
				} 
			}
		}

		origem = u;
		destino = v;
		if (origem>destino){
			origem = v;
			destino =  u;
		}
		edges[contArestasArvore][0] = origem;
		edges[contArestasArvore][1] = destino;
		f[0] += f(0,edges[contArestasArvore][0],edges[contArestasArvore][1]);
		f[1] += f(1,edges[contArestasArvore][0],edges[contArestasArvore][1]);
		contArestasArvore++;
	}

    /* Faz o crossover entre dois individuos.
    Gera so um filho, mas poderia gerar dois */
	void crossover(const SolucaoEdgeSet &pai, const SolucaoEdgeSet &mae) {
		int n =  IRandom(1,NUMEROVERTICES-2); // n pontos 
		// cout<<"n = "<<n<<endl;
		int troca = round((NUMEROVERTICES-2)/n);
		// cout<<"troca = "<<troca<<endl;
		bool b = true; ///true=pai, true=mae
		for (int i=0, cont=0; i<NUMEROVERTICES-2; i++){
			if(b==true)
				pruffer[i] = pai.pruffer[i];
			else 
				pruffer[i] = mae.pruffer[i];
			cont++;
			if (cont==troca){
				cont = 0;
				b = !b;
			}
		}
		convertToTree();
	}



	

	void mutacao(SolucaoEdgeSet &sol){
		for (int i=0; i<NUMEROVERTICES-2; i++){
			pruffer[i] = sol.pruffer[i];
		}
		int n =  IRandom(0,NUMEROVERTICES-3); // um alelo
		int v = IRandom(0,NUMEROVERTICES); 
		pruffer[n] = v;
		convertToTree();
	}

	void operator=(SolucaoEdgeSet &s) {
		nEdges = s.nEdges;
		f[0] = s.f[0];
		f[1] = s.f[1];
		memcpy(edges,s.edges,sizeof(edges));
		for (int i=0; i<NUMEROVERTICES-2; i++){
			pruffer[i] = s.pruffer[i];
		}
		antigof[0] = s.antigof[0];
		antigof[1] = s.antigof[1];
		posicaoListaNSGAII = s.posicaoListaNSGAII; // guarda o index onde a soluçao é guardada na popupacao NUMPOPULACAO*2 do NSGA-II
		prank = s.prank; // diz em que front a solucao está
		isVisitada = s.isVisitada; // flag para o 
		distance = s.distance;
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


	std::vector<SolucaoEdgeSet*> getVizinhos(){ // NUMEROVERTICES vizinhos retornados
		std::vector<SolucaoEdgeSet*> ret;
		int n =  IRandom(0,NUMEROVERTICES-3); // um alelo
		for (int i=0; i<NUMEROVERTICES; i++){
			if (pruffer[n]!=i){
				SolucaoEdgeSet* novo = new SolucaoEdgeSet(NUMEROVERTICES-1);
				for (int j=0; j<NUMEROVERTICES-2; j++){
					novo->pruffer[j] = pruffer[j];
				}
				novo->pruffer[n] = i;
				novo->convertToTree();;
				ret.push_back(novo);
			}
		}
		return ret;
	}

	bool isTree(){ // verificador
		//cout<<"Teste  = ";
		UnionFind uf;
		bool vetkktk[NUMEROVERTICES];
		for(int i=0; i<NUMEROVERTICES; i++)vetkktk[i] = false;
		for (int i=0; i<NUMEROVERTICES-1; i++){
			if (uf.sameClass(this->edges[i][0],this->edges[i][1])==false){
				vetkktk[this->edges[i][0]] = true;
				vetkktk[this->edges[i][1]] = true;
				uf.unionClass(this->edges[i][0],this->edges[i][1]);
			} else {
				cout<<"ERROROROROROROROROROROROROROR TREEEE"<<endl;
				return false;
			}
		}
		for(int i=0; i<NUMEROVERTICES; i++){
			if (vetkktk[i]==false) {
				cout<<"ERROROROROROROROROROROROROROR TREEEE"<<endl;
				
				return false;
			}
		}
		cout<<"It's a tree! õ/"<<endl;;
		return true;
	}

};

#endif
