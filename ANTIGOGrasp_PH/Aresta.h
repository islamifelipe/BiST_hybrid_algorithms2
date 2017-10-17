#ifndef _ARESTA_H_
#define _ARESTA_H_
//using namespace std;

class Aresta{ //OK
	private:
		int origem, destino; /*se o grafo for n�o direcionado, n�o h� diferen�a pr�tica entre os v�rtices a origem e o destino*/
		int id; /*identificador da aresta*/
		float peso1, peso2; /*pesos da aresta*/
		
	public:
		Aresta(int id2, int origem1, int destino1, float peso1_1, float peso2_1); 
		Aresta();	
		int getId();
		int getOrigem();
		int getDestino();
		float getPeso1();
		float getPeso2();
		void setId(int novo);
		void setOrigem(int n);
		void setDestino(int n);
};

#endif

