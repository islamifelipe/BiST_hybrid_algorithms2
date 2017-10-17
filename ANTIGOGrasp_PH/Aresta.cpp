#include "Aresta.h"
using namespace std;

Aresta::Aresta(int id2, int origem1, int destino1, float peso1_1, float peso2_1){
	origem = origem1;
	destino = destino1;
	peso1 = peso1_1;
	peso2 = peso2_1;
	id= id2;
	//pesoUnique = 0;
} 

Aresta::Aresta(){}

int Aresta::getId(){
	return id;
}
int Aresta::getOrigem(){
	return origem;
}
int Aresta::getDestino(){
	return destino;
}
float Aresta::getPeso1(){
	return peso1;
}
float Aresta::getPeso2(){
	return peso2;
}
void Aresta::setId(int novo){
	id = novo;
}
void Aresta::setOrigem(int n){
	origem = n;
}
void Aresta::setDestino(int n){	
	destino = n;	
}