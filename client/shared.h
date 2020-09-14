#ifndef _SHARED_H_
#define _SHARED_H_
#include "edge.h"
struct Shared {
	int cursorX;	//Position of cursor along X axis.
	int cursorY;	//Position of cursor along Y axis.
	int numEdges;	//Number of edges.
	int mazeDim;	//Maze dimension
	double userTime;//Time user takes to traverse maze
	int velx;		//X velocity of cursor
	int vely;		//Y velocity of cursor
	Edge edges[100];//Array of edges.
	uint32_t score;	//Player score
	int numPassed;	//Number of passed edges
	Edge passedEdges[70];//All passed edges
	Edge currentEdge;//Edge the cursor is currently in.
	int startVert;	//Start Vertex in maze
	int endVert;	//End Vertex in maze
	Edge solutionPath[70];//Fastest path
	int solutionSize;	//Vertices in fastest path

};

#endif