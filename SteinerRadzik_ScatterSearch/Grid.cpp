#include "Grid.h"
#include <iostream>
using namespace std;

int Grid::getPositionCount(int p) {
	if (p < 0 || p >= tam) {
		return -1;
	}
	return grid[p];
}
void Grid::addGrid(int p) {
	grid[p]++;
}
void Grid::removeGrid(int p) {
	grid[p]--;
}
void Grid::clearGrid() {
	for (int i=0; i<tam; i++){
		grid[i] = 0;
	}
}
int Grid::getSize() {
	return tam;
}