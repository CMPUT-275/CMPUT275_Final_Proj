/*-------------------------------------------------------------------------
 CMPUT 275 | Winter 2020
 Names: Saif Husnain, Landyn David, Justin Javier, Darius Fang
 CCIDs: sahusnai, lpugh, jjavier, dfang1
 IDs: 1573497, 1586291, 1573754, 1570975

 Final Major Project

 Description: Maze Game
 maze_generator.h
-------------------------------------------------------------------------- */
#ifndef _MAZE_GEN_H_
#define _MAZE_GEN_H_

#include <map>
#include "digraph.h"
#include "wdigraph.h"
#include "heap.h"

// PIL for Dijkstra algorithm
typedef pair<int, int> PIL;

/*
Maze Generation
	x: Height Size
	y: Width Size
	graph: Graph of the Maze
	connected: All edges
	Example of Connected:
		for (auto it = connected.begin(); it != connected.end(); it++) {
			cout << connected->first << " " << connected->second << endl;
			//Outputs the 2 vertexes that are connected
		}
*/

/*
Depth First Search
Traverses neighbours randomly
*/
void dfs_maze(int x, int y, Digraph &graph, multimap<int, int> &connected);

/*
Dijkstra Algorithm
Random Weights on Edges
*/
void djk_maze(int x, int y, Digraph &graph, multimap<int, int> &connected);

/*
Kruskal Algorithm
Randomly Assigned Weights
*/
void kruskal_maze(int x, int y, Digraph &graph, multimap<int, int> &connected);

//Generate Start and End Node Number
void start_end_node(int x, int y, int &start, int &end, Digraph &graph);

//Calculate Score
int calculate_score(int time, int len_answer, int len_solution, Digraph graph);

#endif