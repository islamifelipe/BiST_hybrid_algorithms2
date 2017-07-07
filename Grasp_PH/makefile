
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o grid.o arquivo.o graspph

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp -o Aresta.o -O3 -w -Wall -mms-bitfields 
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp -o Vertice.o -O3 -w -Wall -mms-bitfields 
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp -o Grafo.o -O3 -w -Wall -mms-bitfields 
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp -o Conjunto.o -O3 -w -Wall -mms-bitfields 
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp -o kruskal.o -O3 -w -Wall -mms-bitfields 

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp -o Heap.o -O3 -w -Wall -mms-bitfields 

margeSort.o : margeSort.cpp margeSort.h
	g++ -g -c margeSort.cpp -o margeSort.o -O3 -w -Wall -mms-bitfields 

main.o : main.cpp
	g++ -g -c  main.cpp -o main.o -O3 -w -Wall -mms-bitfields 

grid.o : Grid.cpp Grid.h
	g++ -g -c  Grid.cpp -o  Grid.o -O3 -w -Wall -mms-bitfields 


arquivo.o : arquivo.cpp arquivo.h
	g++ -g -c  arquivo.cpp -o  arquivo.o -O3 -w -Wall -mms-bitfields 

graspph : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o margeSort.o Heap.o  grid.o arquivo.o
	g++ -o graspph Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o grid.o arquivo.o main.o -O3 -w -Wall -mms-bitfields 
	