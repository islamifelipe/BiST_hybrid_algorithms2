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
#include <fstream>      // std::ifstream 

 
// #include "rmcPrim.cpp"
#include "BoundedParetoSet.cpp"
#include "SolucaoEdgeSet.cpp"
#include "PathRelinking.cpp"

using namespace std;

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
BoundedParetoSet *arquivoLimitadoGlobal; /*ARQUIVO GLOBAL LIMITADO A MAXARCSIZE SOLUCOES*/
BoundedParetoSet *pool; // pool das soluçoes que irao ser submetidas ao Peth relinking
std::vector<auxEdgeStruct> listaArestas;
double lambda;

struct tms tempoAntes, tempoDepois;

int numvetores_graaspp; //NUMVETORES_GRASP
int intervalo_prrr; //INTERVALO_PR
int maax_lss; //MAX_LS
int maxx_viz; //MAX_VIZ
int maxprr; //maxPR

void input(const char *instanceName){
    std::ifstream inn;
    inn.open(instanceName, std::ifstream::in);
    int n;
    inn>>n;
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
			inn>>org;
			inn>>dest;
			// org--;
			// dest--;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<NUMOBJETIVOS; ob++){
				inn>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j];

			}
			auxEdgeStruct edgeBiobjetivo = {i,j,custos[0][i][j],custos[1][i][j]};
			listaArestas.push_back( edgeBiobjetivo ); // ATENCAO: BI-OBJETIVO
		}
	}
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

		} while (r<maax_lss && s_was_modified == false);
		
		contIteracoes++;
	}while (contIteracoes<maxx_viz && s_was_modified == true);
	// TODO: delete s_linha ou alocar fora 
	return ret;
}



bool compara(auxEdgeStruct s1, auxEdgeStruct s2){
	return (s1.peso1*lambda + s1.peso2*(1.0-lambda)) < (s2.peso1*lambda + s2.peso2*(1.0-lambda)) ;
}

void grasp(){
	

	/*grasp : ordena as arestas (BI-OBJETIVO) de acordo com um lambda 
	limita o LRC com tamanho max(1, alfa*listaArestas.size())  */
	int conttt = 0;
	for (int itera = 0; itera<numvetores_graaspp; itera++){
		 conttt++;
		lambda = (double) itera/(numvetores_graaspp-1.0);
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
		
		buscaLocal(&novaSol); /// a buscaLocal ja adiciona ao arquivoLimitadoGlobal
		pool->adicionarSol(&novaSol);

		if (conttt == intervalo_prrr){
			conttt = 0;
			//cout<<"pool->getSize() = "<<pool->getSize()<<endl;
			//PR
			list<SolucaoEdgeSet *> listPool = pool->getElementos();
			list<SolucaoEdgeSet *>::iterator it1 = listPool.begin();
			
			for (;it1!=listPool.end();it1++){
				list<SolucaoEdgeSet *>::iterator it2 = it1;
				it2++;
				for (;it2!=listPool.end();it2++){
				//	cout<<"arquivoLimitadoGlobal->getSize() = "<<arquivoLimitadoGlobal->getSize()<<endl;
					pathrelinking(*it1, *it2);
				//	cout<<"arquivoLimitadoGlobal->getSize() = "<<arquivoLimitadoGlobal->getSize()<<endl;
				}
			}
			pool->clear();
		}
	}
	
}



/*semente, arquivo pareto, tempo, lixeira*/

 int main(int argc, char *argv[]){

 	char *instanceName = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-NUMVETORES_GRASP") == 0) {
                numvetores_graaspp = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-INTERVALO_PR") == 0) {
                intervalo_prrr = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-MAX_LS") == 0) {
                maax_lss = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-MAX_VIZ") == 0) {
                maxx_viz = atoi(argv[i+1]);
                i++;
        }
        if (strcmp(argv[i], "-maxPR") == 0) {
                maxprr = atoi(argv[i+1]);
                i++;
        }
        
        if (strcmp(argv[i], "-instance") == 0 || strcmp(argv[i], "-i") == 0) {
                instanceName = argv[i+1];
                i++;
        }
    }
    int semente = time(NULL);
	init_genrand64(semente );
	
	input(instanceName); // ler instância

	arquivoLimitadoGlobal = new BoundedParetoSet();
	pool = new BoundedParetoSet();
	
	grasp();

	// arquivoLimitadoGlobal->printSetPoints(stdout);
	//cout<<"semente = "<<semente<<endl;
	// arquivoLimitadoGlobal->printSetPoints(samplefile);
	// cout<<"Size = "<<arquivoLimitadoGlobal->getSize()<<endl;
	std::ofstream outt;
	string namePareto  = "pareto"+std::to_string(semente)+"_"+std::to_string(NUMEROVERTICES)+".txt";
    string nameHyp = "hyp"+std::to_string(semente)+"_"+std::to_string(NUMEROVERTICES)+".txt";
    outt.open(namePareto, std::ofstream::out);
	list<SolucaoEdgeSet *> lisd = arquivoLimitadoGlobal->getElementos();
	list<SolucaoEdgeSet *>::iterator i = lisd.begin();
	SolucaoEdgeSet *s;
	while (i != lisd.end()) {
		s = *i;
		//fprintf(outt,"%.10lf %.10lf\n",s->getObj(0),s->getObj(1));
		outt<<s->getObj(0)<<" "<<s->getObj(1)<<endl;
		i++;
	}
	string cmd = "./hyp_ind hyp_ind_param_Fev.txt "+ namePareto +" refereceFiltredE3.out "+ nameHyp;
	system(cmd.c_str());
	std::ifstream innn;
    innn.open(nameHyp, std::ifstream::in);
    double hypervolume;
	innn>>hypervolume; // hypervolume;
	fprintf(stdout,"%.9e\n",hypervolume);
	return 0;

}