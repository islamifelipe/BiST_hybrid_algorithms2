#ifndef SOLUCAO_CPP
#define SOLUCAO_CPP

/*This code file was kindly provided by Monteiro */

#include "param.h"
class Solucao {
	protected:
	double f[NUMOBJETIVOS];

	public:
	Solucao() {}

	void setObj(int i, double v) { // set the i-th objective with value v
		f[i] = v;
	}

	void incrementeF(int i, double v) { // increments the i-th objective with value v
		f[i] += v;
	}
	void decrementeF(int i, double v){
		f[i] -= v;
	}

	// Bi-objetivo
	bool operator>> (Solucao &d) { // testa se this domina d
		double ele[2] = {d.getObj(0),d.getObj(1)}, diff[2] = { ele[0] - f[0] , ele[1] - f[1] };
		if (diff[0] > EPS && (diff[1] > EPS || fabs(diff[1]) < EPS))
			return true;
		if ((diff[0] > EPS || fabs(diff[0]) < EPS) && diff[1] > EPS)
			return true;
		return false;
		
	}

	const double getObj(int o) {
		return f[o];
	}

	void operator=(Solucao &s) { // assign s to this object 
		//rg = s.rg;
		for (int i=0; i<NUMOBJETIVOS; i++){
			f[i] = s.f[i];
		}
	}
	bool operator==(Solucao &s) {
		for (int i=0; i<NUMOBJETIVOS; i++){
			if (fabs(s.getObj(i)-getObj(i)) >= EPS) return false;
		}
		return true;
	}
};

#endif
