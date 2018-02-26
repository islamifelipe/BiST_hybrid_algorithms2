
all: Aresta.o Vertice.o Grafo.o main

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp -o Aresta.o -O3 -w -Wall -mms-bitfields 

Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp -o Vertice.o -O3 -w -Wall -mms-bitfields 
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp -o Grafo.o -O3 -w -Wall -mms-bitfields 
	

main.o : main.cpp
	g++ -g -c  main.cpp -o main.o -O3 -w -Wall -mms-bitfields 
	
main : main.o  Grafo.o Vertice.o Aresta.o 
	g++ -o main Aresta.o  Vertice.o Grafo.o  main.o -O3 -w -Wall -mms-bitfields 
	