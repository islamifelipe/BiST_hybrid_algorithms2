
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o steinerRadzik

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp

margeSort.o : margeSort.cpp margeSort.h
	g++ -g -c margeSort.cpp

corley.o : corley.cpp corley.h
	g++ -g -c corley.cpp

main.o : main.cpp
	g++ -lpthread -g -c  main.cpp
		
steinerRadzik : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o margeSort.o Heap.o 
	g++ -lpthread -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o main.o -lm -o steinerRadzik
	