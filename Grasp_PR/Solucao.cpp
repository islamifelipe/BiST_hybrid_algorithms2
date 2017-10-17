#ifndef SOLUCAO_CPP
#define SOLUCAO_CPP

/*This code file was kindly provided by Monteiro */

#include "rand/randomc.h"
#include "param.h"
class Solucao {
	protected:
	double f[NUMOBJETIVOS];
public:
	TRandomMersenne *rg;

	public:
	void setObj(int i, double v) { // set the i-th objective with value v
		f[i] = v;
	}

	public:
	Solucao() {}

	bool operator>> (Solucao &d) { // testa se this domina d
		bool ge = false;
		for (int i=0; i<NUMOBJETIVOS; i++){ 
			if (f[i] < d.getObj(i)) {//// se existe pelo menos um objetivo de this que é extritamente menor que d  
				ge = true;
			}
			if (!(f[i] <= d.getObj(i))) { // se todos os objetivos de this sao menores ou iguais aos objetivos de d
				return false;
			}
		}
		return ge;
		


		// double ele[2] = {d.getObj(0),d.getObj(1)}, diff[2] = { ele[0] - f[0] , ele[1] - f[1] };
		// if (diff[0] > EPS && (diff[1] > EPS || fabs(diff[1]) < EPS))
		// 	return true;
		// if ((diff[0] > EPS || fabs(diff[0]) < EPS) && diff[1] > EPS)
		// 	return true;
		// return false;
	}

	const double getObj(int o) {
		return f[o];
	}

	void operator=(Solucao &s) { // assign s to this object 
		rg = s.rg;
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
