/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017.2
#-----------------------------------------------------------------------
# This code implements a hybrid GRASP algorithm with Path Relinking
#=======================================================================


	==> INSPIRADO NO ARTIGO DE Martí el al (2014) (Multiobjective GRASP with Path Relinking)
	calcula os vetores de escalarizacao
	Grasp{ (Baseado em parte no trabalho de Arroyo e Vianna)
		Constroi solucao 
		Busca local (Vizinhança: remove uma aresta "e" randômica e adiciona uma aresta "e'" randômica que tenha custo escalarizado menor que "e" .)
			==> A parte PR é baseada no trabalho de Goldbarg, Goldbarg e Farias (2007) (aplicou ao caixeiro)
		Após INTERVALO_PR iteraçoes do grasp, monta-se um pool com INTERVALO_PR soluçoes
		Em sequigda, aplica-se o PR truncado (Resende e Ribeiro (2005) e Laguna and Marti (1999)) no pool
		Toma-se pares de soluçoes do pool que estao mais proximas
		métrica para dizer se duas soluçoes do pool sao proximas: ??
	}

*/

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sys/times.h>
#include <sys/types.h>
#include <iostream>
#include <math.h>       /* acos, cos */
#include <climits>
#include <list>

 
// #include "rmcPrim.cpp"
#include "BoundedParetoSet.cpp"
#include "SolucaoEdgeSet.cpp"

using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
BoundedParetoSet *arquivoLimitadoGlobal;
std::vector<auxEdgeStruct> listaArestas;
double lambda;
/*ARQUIVO GLOBAL LIMITADO A MAXARCSIZE SOLUCOES*/

void input(){
	int n; // esta leitura de n é somente para cumprir o formato da instância. O valore de fato está em param.h
	cin>>n;
	int org, dest;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				custos[ob][i][j] = 10e9;
				custos[ob][j][i] = 10e9;
			}
		}
	}
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			cin>>org;
			cin>>dest;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				cin>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j];

			}
			auxEdgeStruct edgeBiobjetivo = {i,j,custos[0][i][j],custos[1][i][j]};
			listaArestas.push_back( edgeBiobjetivo ); // ATENCAO: BI-OBJETIVO
		}
	}
}

/*retorna a quantidade de arestas que existem em s1 que nao existem em s2*/
int distancia(SolucaoEdgeSet s1, SolucaoEdgeSet s2){
	int cont=0;
	for (int i=0; i<NUMEROVERTICES-1; i++){
		bool ha = false;
		for (int j=0; j<NUMEROVERTICES-1 && ha==false; j++){
			if ((s2.edges[j][0]==s1.edges[i][0] && s2.edges[j][1]==s1.edges[i][1]) || (s2.edges[j][0]==s1.edges[i][1] && s2.edges[j][1]==s1.edges[i][0])){
				ha = true;
			}
		}	
		if (ha==false) cont++;
	}
	return cont;
}


//retorna “verdadeiro” caso sl esteja em uma regiao menos populosa que s com relacao ao arquivo local_arc.
bool m_grid(SolucaoEdgeSet *sl, SolucaoEdgeSet *s){
	return arquivoLimitadoGlobal->getPositionCount(*sl)<arquivoLimitadoGlobal->getPositionCount(*s);
}

/*a busca tabu edita a solucao 's' que foi passada como argumento*/
bool buscaLocal(SolucaoEdgeSet *s){
	int contIteracoes = 0;
	bool s_was_modified = false;
	bool ret = false;
	int r; // do pseudo-codigo
	SolucaoEdgeSet *s_linha = new SolucaoEdgeSet(NUMEROVERTICES-1);
	
	do{ // repita até contIteracoes==MAX_LS or s nao seja modificado
		r = 0;
		do{	
			r++;
			s_linha->getVizinho1(NUMEROVERTICES-1-1,*s);
			
			//if(s_linha->getObj(0)*lambda + s_linha->getObj(1)*(1.0 - lambda) < s->getObj(0)*lambda + s->getObj(1)*(1.0 - lambda)){
			s_was_modified = arquivoLimitadoGlobal->adicionarSol(s_linha);
			if (s_was_modified==true){ // critério de aceitaçao
				*s = *s_linha;
				ret = true;
			} 

		} while (r<MAX_LS && s_was_modified == false);
		
		contIteracoes++;
	}while (contIteracoes<MAX_VIZ && s_was_modified == true);
	return ret;
}



// void buscaLocal(SolucaoEdgeSet &sol, double lambda){
// 	SolucaoEdgeSet xk(NUMEROVERTICES-1);
// 	SolucaoEdgeSet nova(NUMEROVERTICES-1);
// 	xk = sol;
// 	for (int i=0; i<MAX_LS; i++){
// 		//double min = xk.getObj(0)*lambda + xk.getObj(1)*(1.0-lambda);
// 		SolucaoEdgeSet minSol = xk;
// 		for (int viz=0; viz<MAX_VIZ; viz++){
// 			//gerou o vizinho
// 			a1 = rg.IRandom(0,NUMEROVERTICES-1-1);
			// while ((a2 = rg.IRandom(0,NUMEROVERTICES-1-1)) == a1);
			// nova.trocaArestas(a1,a2,nova.calcularTrocaArestas(a1,a2,xk),xk);

// 			bool added = arquivoLimitadoGlobal->adicionarSol(&nova);
// 			if (added==true){
// 				//min = custonovo;
// 				// cout<<"OKFOEKFOEKFOEKFOEKFOEKFOEKFOK"<<endl;
// 				// cout<<distancia(minSol, nova)<<endl;
// 				minSol = nova;
// 			}
// 		}
// 		//cout<<"min = "<<min<<endl;
// 		xk = minSol;
// 	}
// }




bool compara(auxEdgeStruct s1, auxEdgeStruct s2){
	return (s1.peso1*lambda + s1.peso2*(1.0-lambda)) < (s2.peso1*lambda + s2.peso2*(1.0-lambda)) ;
}

void grasp(){
	

	/*grasp : ordena as arestas (BI-OBJETIVO) de acordo com um lambda 
	limita o LRC com tamanho max(1, alfa*listaArestas.size())  */

	for (int itera = 0; itera<NUMVETORES_GRASP; itera++){
		lambda = (double) itera/(NUMVETORES_GRASP-1.0);
		//cout<<lambda<<" "<<1.0-lambda<<endl;
		//cout<<"Itaracao = "<<itera<<endl;
		sort(listaArestas.begin(), listaArestas.end(), compara);
		
		int rlc = (int) ALFA*listaArestas.size(); // tamanho da LRC
		if (rlc<1) rlc = 1;

		std::vector<auxEdgeStruct> LRC; // guarda indexes de listaAresta
		for (int li = 0; li<rlc; li++) LRC.push_back(listaArestas[li]);
		
		UnionFind uf;
		SolucaoEdgeSet novaSol(NUMEROVERTICES-1);
		int edgedAdded = 0;
		while (edgedAdded<NUMEROVERTICES-1 && LRC.size()>0){
			int index = IRandom(0, LRC.size()-1); // entre 0 e LRC.size()-1
			auxEdgeStruct a = LRC[index];
			if (uf.sameClass(a.origem,a.destino)==false){
				novaSol.edges[edgedAdded  ][0] = a.origem;
				novaSol.edges[edgedAdded++][1] = a.destino;
				uf.unionClass(a.origem,a.destino);
			} 
			LRC.erase(LRC.begin()+index);
		}

		if (edgedAdded<NUMEROVERTICES-1){ // se, por acaso, o laço acima terminar sem uma arvore estar formada, entao adcionamos as menores arestas
			for (int i=0; i<listaArestas.size() && edgedAdded<NUMEROVERTICES-1; i++){
				auxEdgeStruct a = listaArestas[i];
				if (uf.sameClass(a.origem,a.destino)==false){
					novaSol.edges[edgedAdded  ][0] = a.origem;
					novaSol.edges[edgedAdded++][1] = a.destino;
					uf.unionClass(a.origem,a.destino);
				} 
			}
		}

		novaSol.calcularObjetivos();
		arquivoLimitadoGlobal->adicionarSol(&novaSol);
		
		// SolucaoEdgeSet *s_linha = new SolucaoEdgeSet(NUMEROVERTICES-1);
		// s_linha->getVizinho1(NUMEROVERTICES-1-1,novaSol, lambda);
		bool resulBL = buscaLocal(&novaSol); /// a buscaLocal ja adiciona ao arquivoLimitadoGlobal
		cout<<"resulBL = "<<resulBL<<endl;
	}
	
}



/*semente, arquivo pareto, tempo, lixeira*/

 int main(int argc, char *argv[]){
	int seemente = std::atoi(argv[1]);
	init_genrand64(seemente);
	cout<<"========= Estatisticas ========= "<<endl;
	cout<<"Semente utilizada : "<<seemente<<endl;
	FILE *samplefile = fopen(argv[2],"a");
	FILE *tempofile = fopen(argv[3],"a");
	char *lixeira = argv[4];
	input(); // ler instância

	arquivoLimitadoGlobal = new BoundedParetoSet();
	arquivoLimitadoGlobal->setNomeGlobalf(lixeira);

	
	grasp();

	arquivoLimitadoGlobal->printSetPoints(stdout);
	cout<<"Size = "<<arquivoLimitadoGlobal->getSize()<<endl;



	fclose(samplefile);
	fclose(tempofile);
}