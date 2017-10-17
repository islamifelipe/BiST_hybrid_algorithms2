#ifndef _GRID_H_
#define _GRID_H_

#define PROFUNDIDADEGRID 5

/*This code was adapted from Monteiro et al (2010)*/

class Grid {
	private:
		const static int tam = 1<<(2*PROFUNDIDADEGRID);
		int grid[1<<(2*PROFUNDIDADEGRID)];
	public:
		int getPositionCount(int p);
		void addGrid(int p);
		void removeGrid(int p);
		void clearGrid();
		int getSize();
};

#endif
