#ifndef _ARQUIVO_H_
#define _ARQUIVO_H_

/*This code was adapted from Monteiro et al (2010)*/

#include <list>
#include <map>
#include "Grid.h"
#include <algorithm>    // std::max
#include <utility>      // std::pair

using namespace std;

typedef struct {
	float min, max;
} range;

class Arquivo {
	private:
		list <pair<int*, pair<float, float> > > soluces;
		range rangeNovo[2], rangeAtual[2];
		Grid g;

		int calcularGridPos(pair<int*, pair<float, float> > s);
		void updateGrid() ;
		void reiniciarRanges();

	public:
		Arquivo();
		~Arquivo();

		int getPositionCount(pair<int*, pair<float, float> > s);

		int getPositionCount(int p);

		list<pair<int*, pair<float, float> > > getSolucoes();


		bool adicionarSol(pair<int*, pair<float, float> > s);
		Grid getGrid();
		void removeSolution(int pos);

		int getSize();

		bool confereGrid();
};

#endif
