/*-------------------------------------------------------------------------
 CMPUT 275 | Winter 2020
 Names: Saif Husnain, Landyn David, Justin Javier, Darius Fang
 CCIDs: sahusnai, lpugh, jjavier, dfang1
 IDs: 1573497, 1586291, 1573754, 1570975

 Final Major Project

 Description: Maze Game
 maze_solver.cpp
-------------------------------------------------------------------------- */
#include "maze_solver.h"


void solve_maze(int startVertex, int endVertex, Digraph &graph, vector<int> &answer) {
	unordered_map<int, int> reached;
	queue<int> q;
	int u;
	reached[startVertex] = startVertex;
	q.push(startVertex);
	while (!q.empty()) {
		u = q.front();
		q.pop();
		for (auto it = graph.neighbours(u); it != graph.endIterator(u); it++) {
			if (reached.find(*it) == reached.end()) {
				reached[*it] = u;
				q.push(*it);
			}
		}
	}
	int next;
	answer.push_back(endVertex);
	while (true) {
		next = reached[endVertex];
		answer.push_back(next);
		endVertex = next;
		if (next == startVertex) {
			break;
		}
	}
}