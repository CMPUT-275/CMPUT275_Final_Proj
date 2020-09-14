/*-------------------------------------------------------------------------
 CMPUT 275 | Winter 2020
 Names: Saif Husnain, Landyn David, Justin Javier, Darius Fang
 CCIDs: sahusnai, lpugh, jjavier, dfang1
 IDs: 1573497, 1586291, 1573754, 1570975

 Final Major Project

 Description: Maze Game
 maze_solver.h
 
-------------------------------------------------------------------------- */
#ifndef _MAZE_SOLVER_H
#define _MAZE_SOLVER_H
#include <queue>
#include <vector>
#include "digraph.h"

/*
	Solves the give maze using breath first search

	startVerex: Start Vertex Number
	endVertex: End Vertex Number
	graph: Digraph of the maze
	answer: empty vector, will be populated with the answer
*/
void solve_maze(int startVertex, int endVertex, Digraph &graph, vector<int> &answer);

#endif