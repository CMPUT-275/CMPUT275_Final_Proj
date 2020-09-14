#ifndef _EDGE_H
#define _EDGE_H

/* CONTAINS EDGE STRUCT USE TO STORE ALL EDGES IN GRAPH */
struct Edge {
	bool isStart;
	bool isEnd;
	int edgeStart;		//Starting vertex of edge
	int edgeEnd;		//Ending vertex of edge
	int startCoordX;	//X coordinate of starting vertex
	int endCoordX;		//X coordinate of ending vertex
	int startCoordY;	//Y coordinate of starting vertex
	int endCoordY;		//Y coordinate of ending vertex
	
};

#endif
