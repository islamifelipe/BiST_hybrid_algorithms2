
#ifndef AUXEDGESTRUCT_H
#define AUXEDGESTRUCT_H

/*This code file was kindly provided by Monteiro */

struct auxEdgeSet {

	int a; // origem
	int b; // destino
	double fit; // fitness de acordo com um vetor de escalarizaçao

};

inline bool auxEdgeSetComp (auxEdgeSet e1, auxEdgeSet e2) {

	return (e1.fit < e2.fit);
}

struct auxEdgeStruct {

	int a;
	int b;
	double c1;
	double c2;
	double fit;

};

struct fitVecNode {
	int a, b;
	double c;
};

// struct simpleEdge {
// 	int a, b;
// 	double fit;
// };

struct ordenacao {
	int id; //identificador
	double v_esc [NUMOBJETIVOS]; //vetor de escalarização associado
	auxEdgeSet arestas [(NUMEROVERTICES*(NUMEROVERTICES-1))/2]; //conjunto ordenado de arestas de acordo com o v_esc
};

#endif
