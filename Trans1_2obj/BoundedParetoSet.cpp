#ifndef BOUNDEDPARETOSET_CPP
#define BOUNDEDPARETOSET_CPP

#include "ParetoSet.cpp"
#include <cstdio>
#include <string>
#include <list>

/*This code file was kindly provided by Monteiro */


using namespace std;

extern int objetivoOrdenacao; // esta variavel é utilizada para designar qual objetivo será utilizado para ordenar as soluçoes

bool compare1(SolucaoEdgeSet *s1, SolucaoEdgeSet *s2){
	return s1->getObj(objetivoOrdenacao) < s2->getObj(objetivoOrdenacao);
}

class BoundedParetoSet : public ParetoSet {
	private:
	const static int MAXARCSIZE = 300;
 	//FILE *globalf;
	//bool existeFileGlobal;
    //string nomeglobalf;
	
	public:
	list<SolucaoEdgeSet *> lixeira;

	/* Complexidade: O(n) */
	BoundedParetoSet () {
		///existeFileGlobal = false;
	}
	~BoundedParetoSet () {
		// if (existeFileGlobal) {
		// 	fclose(globalf);
		// }
	}

    // void setNomeGlobalf (char * n) {
    //     nomeglobalf = n;
    // }

	bool adicionarSol(SolucaoEdgeSet *s) {
		//ASS ( assert( confereGrid() ); )
		list<SolucaoEdgeSet *>::iterator maisPopuloso = sol.begin();
		int maiorPositionCount = -1;
		if (sol.size() > 0) maiorPositionCount = getPositionCount( **sol.begin() );
		
		/* percorre o vetor de solucoes e de valores e, caso exista solucao dominada, retira e retorna true. caso contrario, retorna false */
		list<SolucaoEdgeSet *>::iterator i = sol.begin();
		list< list<SolucaoEdgeSet *>::iterator > remover;
		while (i != sol.end()) {
			// se a solucao que vai entrar domina a solucao i 
			if (*s >> **i) {
				remover.push_back(i); 
			}
			// se a solucao que vai entrar nao domina a solucao i, procuramos a solucao que está no
			// local mais populoso, caso o pareto esteja no tamanho maximo
			if (remover.size() == 0 && getSize()+1 > MAXARCSIZE) {
				int positionCountAtual = getPositionCount(**i); // posiçao de i na grid
				if (maiorPositionCount < positionCountAtual) { // pega o maximo entre os dois. Desta posiçao será removida a soluçao para "sol" entrar
					maiorPositionCount = positionCountAtual;
					maisPopuloso = i;
				}
			}
			
			if (**i >> *s || **i == *s){ // caso algum "i" domine "sol", para e retorna false
				lixeira.push_back( s );
				return false;
			}
			i++;
		}

		// // se a solucao que vai entrar nao domina nenhuma e o tamanho do conjunto pareto ja esta no maximo
		// // (se nenhuma solucao vai sair do conjunto), remove a mais populosa
		// if (remover.size() == 0 && getSize()+1 > MAXARCSIZE) {
		// 	//fprintf(stderr,"BoundedPareto: removendo a solucao mais populosa de pop = %d\n",maiorPositionCount);
		// 	if (!existeFileGlobal) {
		// 		globalf = fopen (nomeglobalf.data(), "a");
		// 		existeFileGlobal = true;
		// 	}
		// 	else {
		// 		if (globalf == NULL)
		// 			cerr << "erro ao abrir arquivo!!" << endl;
		// 		else
		// 			printOnePoint (maisPopuloso, globalf);
		// 	}
		// 	remover.push_back(maisPopuloso);
		// }

		//fprintf(stderr,"getSize = %d %d\n",getSize(),sol.size());
			
		list< list<SolucaoEdgeSet *>::iterator >::iterator j = remover.begin();
		while (j != remover.end()) {
			// remove do grid
			g.removeGrid( calcularGridPos(***j) );
			// remove a frequencia das arestas
			removerFrequencia( **j );
			//delete( **j ); // ?????
			// remove do conjunto pareto
			sol.erase( *j );
			// insere na lixeira // by felipe
			lixeira.push_back( **j );
			j++;
		}

		SolucaoEdgeSet *t = new SolucaoEdgeSet(s->nEdges);
		*t = *s;
		// adiciona ao conjunto pareto
		sol.push_front( t );
		if (sol.size() > MAXARCSIZE) fprintf(stderr,"ERRO!\n");
		// adiciona a frequencia das arestas da solucao
		adicionarFrequencia( t );
		// adiciona ao grid
		g.addGrid( calcularGridPos(*t) );

		for(int k=0;k<NUMOBJETIVOS;k++) {
			rangeNovo[k].min = min(rangeNovo[k].min,t->getObj(k));
			rangeNovo[k].max = max(rangeNovo[k].max,t->getObj(k));
		}

		// se houve uma mudanca grande nos ranges (maior que 10% valor), atualizar o grid
		for (int k=0;k<NUMOBJETIVOS;k++) {
			if (fabs(rangeNovo[k].min-rangeAtual[k].min) > 0.1*rangeAtual[k].min || fabs(rangeNovo[k].max-rangeAtual[k].max) > 0.1*rangeAtual[k].max) {
				//fprintf(stderr,"Atualizando grid!\n");
				updateGrid();
				break;
			}
		}

		//ASS ( assert( confereGrid() ); )
		return true;
	}

	
	void crowndDistance(){ // by Felipe
		#define INF 1e9
		int l = lixeira.size();
		list<SolucaoEdgeSet *>::iterator it = lixeira.begin();
		while (it!=lixeira.end()){
			(*it)->distance = 0;
			it++;
		}
		for (int m=0; m<NUMOBJETIVOS; m++){ // para cada objetivo m
			objetivoOrdenacao = m;
			lixeira.sort(compare1);
			(*lixeira.begin())->distance = INF;
			(lixeira.back())->distance = INF; // É ISSO MESMO: back() NAO retorna um ponteiro interator!!! 
			it = lixeira.begin();
			it++; // começa do 1 (2 do Hudson)
			for (int i = 1; i<(l-1); i++){
				it++; // pos
				double objPos = (*it)->getObj(m);
				it--; // volta para pro original
				it--; //previous
				double objPrev = (*it)->getObj(m);
				it++; // volta para pro original
				(*it)->distance = (*it)->distance + (objPos - objPrev);
				it++; // avança
			}
			// termina no l-2 (end-1) ou (l-1 do Hudson)
		}
		#undef INF
	}
};

#endif
