#include "Arquivo.h"
#include <cmath>       /* fabs */
#include <iostream>
#include "PopulacaoInicial.h"


/*this function calculates the position of the grid where the solution will be inserted into*/
int Arquivo::calcularGridPos(pair<int*, pair<float, float> > s) {
	int bit = 0;
	int gridPos = 0;
	for (int obj=0;obj<2;obj++) {
		float inicio = rangeAtual[obj].min;
		float fim = rangeAtual[obj].max;
		float meio = (float) (inicio+fim)/2.0;
		for (int k=0;k<PROFUNDIDADEGRID;k++) {
			float peso_obj = s.second.first;
			if (obj==1) peso_obj = s.second.second;
			if (peso_obj >= meio) {
				gridPos |= (1 << bit);
				inicio = meio;
			} else {
				fim = meio;
			}
			meio = (float) (inicio+fim)/2.0;
			bit++;
		}
	}
	return gridPos;
}

void Arquivo::updateGrid() {
	g.clearGrid();

	list<pair<int*, pair<float, float> > >::iterator it = soluces.begin();
	reiniciarRanges();
	while (it != soluces.end()) {
		rangeAtual[0].min = rangeNovo[0].min = min(rangeAtual[0].min,(*it).second.first);
		rangeAtual[0].max = rangeNovo[0].max = max(rangeAtual[0].max,(*it).second.first);
		
		rangeAtual[1].min = rangeNovo[1].min = min(rangeAtual[1].min,(*it).second.second);
		rangeAtual[1].max = rangeNovo[1].max = max(rangeAtual[1].max,(*it).second.second);
		
		it++;
	}

	it = soluces.begin();
	while (it != soluces.end()) {
		g.addGrid( calcularGridPos(*it) );
		it++;
	}
}

void Arquivo::reiniciarRanges() {
	for (int k=0;k<2;k++) {
		rangeAtual[k].min = rangeNovo[k].min = 100000000000;
		rangeAtual[k].max = rangeNovo[k].max = -100000000000;
	}
}

Arquivo::Arquivo(){
	reiniciarRanges();
}

Arquivo::~Arquivo() {
	for (list<pair<int*, pair<float, float> > >::iterator it =soluces.begin(); it!=soluces.end(); it++){
		delete[] (*it).first;
	}
	g.clearGrid();
}

int Arquivo::getPositionCount(pair<int*, pair<float, float> > s) {
	return g.getPositionCount( calcularGridPos(s) );
}

int Arquivo::getPositionCount(int p) {
	return g.getPositionCount( p );
}

list<pair<int*, pair<float, float> > > Arquivo::getSolucoes() {
		return soluces;
}

/* Complexidade: O(n) */
bool Arquivo::adicionarSol(pair<int*, pair<float, float> > s) {
	//cout<<"Antes = "<<s.second.first<<" "<<s.second.second<<endl;
	list<pair<int*, pair<float, float> > >::iterator it = soluces.begin();
	list< list<pair<int*, pair<float, float> > >::iterator > remover; // uma lista de interators
	while (it != soluces.end()) {
		if (s.second.first <= (*it).second.first && s.second.second <= (*it).second.second && (s.second.first < (*it).second.first || s.second.second < (*it).second.second)){
			remover.push_back(it); // se s domina it, entao guardamos it para remover caso s seja inserido
		}
		// se it domina s ou se s é igual a it, entao nao acrecenta-se s
		if (((*it).second.first <= s.second.first && (*it).second.second <= s.second.second && ((*it).second.first < s.second.first || (*it).second.second < s.second.second)) || ((*it).second.first == s.second.first && (*it).second.second == s.second.second)) {
			//cout<<"false"<<endl;
			delete[] s.first; // deleta o clone 
			return false;
		}
		it++;
	}

	list< list<pair<int*, pair<float, float> > >::iterator >::iterator it_r = remover.begin();
	while (it_r != remover.end()) {
		// remove do grid
		g.removeGrid( calcularGridPos( (*(*it_r)) ) );
		// remove do arquivo
		delete[] (*(*it_r)).first;
		soluces.erase( (*it_r) );
		it_r++;
	}

	// adiciona ao arquivo
	
	soluces.push_back(s);
	// adiciona ao grid
	g.addGrid( calcularGridPos(s) );

	rangeNovo[0].min = min(rangeNovo[0].min,s.second.first);
	rangeNovo[0].max = max(rangeNovo[0].max,s.second.first);
	
	rangeNovo[1].min = min(rangeNovo[1].min,s.second.second);
	rangeNovo[1].max = max(rangeNovo[1].max,s.second.second);


	// se houve uma mudanca grande nos ranges (maior que 10% valor), atualizar o grid
	for (int k=0;k<2;k++) {
		if (fabs(rangeNovo[k].min-rangeAtual[k].min) > 0.1*rangeAtual[k].min || fabs(rangeNovo[k].max-rangeAtual[k].max) > 0.1*rangeAtual[k].max) {
			updateGrid();
			break;
		}
	}
	//cout<<"Depois = "<<s.second.first<<" "<<s.second.second<<endl;
	//cout<<endl;
	return true;
}

Grid Arquivo::getGrid(){
	return g;
}
int Arquivo::getSize() {
	return soluces.size();
}

void Arquivo::removeSolution(int pos){ // talvez seja preciso fazer uma funcao que tome como argumento a solucao em si, e nao sua posicao
	list<pair<int*, pair<float, float> > >::iterator it = soluces.begin();
	int cont = 0;
	for (;it != soluces.end() && cont<pos; cont++,it++);
	g.removeGrid( calcularGridPos( (*it) ) );
	delete[] (*it).first;
	// remove do arquivo
	soluces.erase( it );
}

bool Arquivo::confereGrid() {
	unsigned s = 0;
	for (int i=0;i<g.getSize();i++) s += g.getPositionCount(i);
	return s == soluces.size();
}