/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017.2
#-----------------------------------------------------------------------
# This code implements a hybrid GRASP algorithm with Path Relinking
#=======================================================================


	==> INSPIRADO NO ARTIGO DE Martí el al (2014) (Multiobjective GRASP with Path Relinking)
	calcula os vetores de escalarizacao a cada iteraçao
	Grasp{ (Baseado em parte no trabalho de Arroyo e Vianna)
		Constroi solucao (kruskal)
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

int countAcceptConstrutivo; // quantidade de soluçoes do constutivo aceitas no arquivo externo 
int countAcceptBL; // quantiade de soluçoes da BL aceitas no arquivo externo
int countAcceptPR; // quantidade de soluçoes do PR aceitas no arquivo externo
int quantConstrutivo; // quantidade de vezes em que o construtivo é executado (= NUMVETORES_GRASP)
int quantBL; // quantidade de vezes em que a BL é executada
int quantPR; // quantiade de vezes em que o PR é executado

//obviamente, é possível que countAcceptBL > quantBL e countAcceptPR > quantPR

// int quantAvalicaoObjetivo; // conta quantas vezes a funçao objetivo foi avaliada

SolucaoEdgeSet *s_linha;

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
			org--;
			dest--;
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


//retorna “verdadeiro” caso sl esteja em uma regiao menos populosa que s com relacao ao arquivo local_arc.
bool m_grid(SolucaoEdgeSet *sl, SolucaoEdgeSet *s){
	return arquivoLimitadoGlobal->getPositionCount(*sl)<arquivoLimitadoGlobal->getPositionCount(*s);
}

/*a busca tabu edita a solucao 's' que foi passada como argumento*/
bool buscaLocal(SolucaoEdgeSet *s){
	quantBL++;
	int contIteracoes = 0;
	bool s_was_modified = false;
	bool ret = false;
	int r; // do pseudo-codigo
	
	do{ // repita até contIteracoes==MAX_LS or s nao seja modificado
		r = 0;
		do{	
			r++;
			s_linha->getVizinho1(NUMEROVERTICES-1-1,*s);
			
			//if(s_linha->getObj(0)*lambda + s_linha->getObj(1)*(1.0 - lambda) < s->getObj(0)*lambda + s->getObj(1)*(1.0 - lambda)){
			
			s_was_modified = arquivoLimitadoGlobal->adicionarSol(s_linha);
			if (s_was_modified==true){ // critério de aceitaçao
				countAcceptBL++;
				*s = *s_linha;
				ret = true;
			} 

		} while (r<MAX_LS && s_was_modified == false);
		
		contIteracoes++;
	}while (contIteracoes<MAX_VIZ && s_was_modified == true);
	return ret;
}



bool compara(auxEdgeStruct s1, auxEdgeStruct s2){
	return (s1.peso1*lambda + s1.peso2*(1.0-lambda)) < (s2.peso1*lambda + s2.peso2*(1.0-lambda)) ;
}

void grasp(){
	

	/*grasp : ordena as arestas (BI-OBJETIVO) de acordo com um lambda 
	limita o LRC com tamanho max(1, alfa*listaArestas.size())  */
	int conttt = 0;
	for (int itera = 0; itera<NUMVETORES_GRASP; itera++){
		 conttt++;
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
		while (edgedAdded<NUMEROVERTICES-1 && LRC.size()>0){ // como o Kruskal
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
		quantConstrutivo++;
		if (arquivoLimitadoGlobal->adicionarSol(&novaSol)==true) countAcceptConstrutivo++;
		
		buscaLocal(&novaSol); /// a buscaLocal ja adiciona ao arquivoLimitadoGlobal
		pool->adicionarSol(&novaSol); /// POOOOOOOOOOOL

		if (conttt == INTERVALO_PR){
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
	int seemente = std::atoi(argv[1]);
	init_genrand64(seemente);
	cout<<"========= Estatisticas ========= "<<endl;
	cout<<"Semente utilizada : "<<seemente<<endl;
	FILE *samplefile = fopen(argv[2],"a");
	FILE *tempofile = fopen(argv[3],"a");
	//char *lixeira = argv[4];
	input(); // ler instância

	arquivoLimitadoGlobal = new BoundedParetoSet();
	pool = new BoundedParetoSet();
	//arquivoLimitadoGlobal->setNomeGlobalf(lixeira);

	s_linha = new SolucaoEdgeSet(NUMEROVERTICES-1);

	countAcceptConstrutivo = 0;
	countAcceptBL = 0;
	countAcceptPR = 0;
	quantConstrutivo = 0;
	quantBL = 0;
	quantPR = 0;

	times(&tempoAntes);
	
	grasp();

	times(&tempoDepois);

	fprintf(stdout,"Tempo(s) Final = %.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
	fprintf(tempofile,"%.2lf\n", (double) (tempoDepois.tms_utime - tempoAntes.tms_utime) / 100.0 );
	cout<<"Quantidade de soluçoes encontradas = "<<arquivoLimitadoGlobal->getSize()<<endl;

	
	cout<<"Quantidade de vezes em que o contrutivo foi executada: "<<quantConstrutivo<<endl;
	cout<<"Quantidade de vezes em que a BL foi executada: "<<quantBL<<endl;
	cout<<"Quantidade de vezes em que o PR foi executado: "<<quantPR<<endl;

	cout<<"Quantidade de solucoes que o constutivo adicionou ao arquivo global: "<<countAcceptConstrutivo<<endl;
	cout<<"Quantidade de solucoes que a BL adicionou ao arquivo global: "<<countAcceptBL<<endl;
	cout<<"Quantidade de solucoes que o PR adicionou ao arquivo global: "<<countAcceptPR<<endl;
	
	cout<<"Quantidade de avaliacoes da funcao objetivo: "<<arquivoLimitadoGlobal->quantAvalicaoObjetivo<<endl;;

	cout<<"Pareto aproximativo: "<<endl;
	arquivoLimitadoGlobal->printSetPoints(stdout);
	arquivoLimitadoGlobal->printSetPoints(samplefile);
	fprintf(samplefile,"\n");


	fclose(samplefile);
	fclose(tempofile);
}