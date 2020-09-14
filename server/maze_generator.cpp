/*-------------------------------------------------------------------------
 CMPUT 275 | Winter 2020
 Names: Saif Husnain, Landyn David, Justin Javier, Darius Fang
 CCIDs: sahusnai, lpugh, jjavier, dfang1
 IDs: 1573497, 1586291, 1573754, 1570975

 Final Major Project

 Description: Maze Game
 maze_generator.cpp
-------------------------------------------------------------------------- */
#include <random> //for rand
#include <algorithm> //for random_shuffle and sort
#include <iostream>
#include <ctime> //for time
#include "maze_generator.h"
#include "maze_solver.h"
#include "digraph.h"
#include "unionfind.h"





void add_random_edge(int x, int y, Digraph &graph, multimap<int, int> &connected, int p) {
	Digraph grid;
	//Create Grid
	for (int i = 0; i < (x*y); i++) {
		grid.addVertex(i);
	}
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x-1; j++) {
			grid.addEdge(j + x*i, j+1 + x*i);
			grid.addEdge(j+1 + x*i, j + x*i);
		}
	}
	for (int i = 0; i < y-1; i++) {
		for (int j = 0; j < x; j++) {
			grid.addEdge(j + x*i, j + x*(i+1));
			grid.addEdge(j + x*(i+1), j + x*i);
		}
	}
	//Add Edges Randomly
	for (int i = 0; i < (x*y); i++) {
		if (rand() % p == 1) {
			for (auto it = grid.neighbours(i); it != grid.endIterator(i); it++) {
				if (rand() % p == 1 && !graph.isEdge(i, *it)) {
					connected.insert(pair<int,int>(i, *it));
					graph.addEdge(i, *it);
					graph.addEdge(*it, i);
				}
			}
		}
	}
}

void depth_first_search(int u, int prev, Digraph &graph, unordered_map<int, int> &reached) {
	if (reached.find(u) != reached.end()) {
		return;
	}
	reached[u] = prev;
	vector<int> neighbours;
	for (auto it = graph.neighbours(u); it != graph.endIterator(u); it++) {
		neighbours.push_back(*it);
	}
	//Randomly Visit Neighbours
	random_shuffle(neighbours.begin(), neighbours.end());
	for (auto it = neighbours.begin(); it != neighbours.end(); it++) {
		depth_first_search(*it, u, graph, reached);
	}
}

void dijkstra(int startVertex, WDigraph &graph, unordered_map<int, PIL> &reached) {
	BinaryHeap<PIL, int> events;
	int u, v;
	PIL edge = {startVertex, startVertex};
	events.insert(edge, 0);
	while(events.size() > 0) {
		auto event = events.min();
		events.popMin();
		if (reached.find(event.item.second) == reached.end()) {
			u = event.item.first;
			v = event.item.second;
			edge = {u, event.key};
			reached[v] = edge;
			for (auto w = graph.neighbours(v); w != graph.endIterator(v); w++) {
				edge = {v, *w};
				events.insert(edge, event.key + graph.getCost(v, *w));
			}
		}
	}
}

void dfs_maze(int x, int y, Digraph &graph, multimap<int, int> &connected) {
	Digraph grid;
	unordered_map<int, int> reached;
	int startVertex = rand() % (x*y); //random vertex to start dfs
	//Create Grid
	for (int i = 0; i < (x*y); i++) {
		grid.addVertex(i);
	}
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x-1; j++) {
			grid.addEdge(j + x*i, j+1 + x*i);
			grid.addEdge(j+1 + x*i, j + x*i);
		}
	}
	for (int i = 0; i < y-1; i++) {
		for (int j = 0; j < x; j++) {
			grid.addEdge(j + x*i, j + x*(i+1));
			grid.addEdge(j + x*(i+1), j + x*i);
		}
	}
	depth_first_search(startVertex, startVertex, grid, reached);
	reached.erase(startVertex); //startVertex with edge to itself
	for (auto it = reached.begin(); it != reached.end(); it++) {
		connected.insert(pair<int,int>(it->first, it->second));
		graph.addEdge(it->first, it->second);
		graph.addEdge(it->second, it->first);
	}
	add_random_edge(x, y, graph, connected, x);
}

void djk_maze(int x, int y, Digraph &graph, multimap<int, int> &connected) {
	WDigraph grid;
	unordered_map<int, PIL> reached;
	int startVertex = rand() % (x*y); //random vertex to start djk
	//create grid with random weights
	for (int i = 0; i < (x*y); i++) {
		grid.addVertex(i);
	}
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x-1; j++) {
			grid.addEdge(j + x*i, j+1 + x*i, rand() % 100);
			grid.addEdge(j+1 + x*i, j + x*i, rand() % 100);
		}
	}
	for (int i = 0; i < y-1; i++) {
		for (int j = 0; j < x; j++) {
			grid.addEdge(j + x*i, j + x*(i+1), rand() % 100);
			grid.addEdge(j + x*(i+1), j + x*i, rand() % 100);
		}
	}
	dijkstra(startVertex, grid, reached);
	reached.erase(startVertex); //startVertex edge with itself
	for (auto it = reached.begin(); it != reached.end(); it++) {
		connected.insert(pair<int,int>(it->first, it->second.first));
		graph.addEdge(it->first, it->second.first);
		graph.addEdge(it->second.first, it->first);
	}
	add_random_edge(x, y, graph, connected, x);
}

//Edge struct for kruskal for sorting
struct Edge {
	int u, v, cost;
	bool operator<(const Edge& rhs) const {
		return cost < rhs.cost;
	}
};

void kruskal_maze(int x, int y, Digraph &graph, multimap<int, int> &connected) {
	UnionFind uf;
	vector<Edge> edges;
	Edge e;
	for (int i = 0; i < (x*y); i++) {
		uf.add_item(i);
	}
	//Add all edges to unionfind with random weights
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x-1; j++) {
			e = {j + x*i, j+1 + x*i, rand() % 100};
			edges.push_back(e);
		}
	}
	for (int i = 0; i < y-1; i++) {
		for (int j = 0; j < x; j++) {
			e = {j + x*i, j + x*(i+1), rand() % 100};
			edges.push_back(e);
		}
	}
	sort(edges.begin(), edges.end());
	for (Edge e: edges) {
		if (uf.merge(e.u, e.v)) { //if no cycle add edge
			connected.insert(pair<int, int>(e.u, e.v));
		}
	}
	for (auto it: connected) {
		graph.addEdge(it.first, it.second);
		graph.addEdge(it.second, it.first);
	}
	add_random_edge(x, y, graph, connected, x);
}

//===============================================================================

void start_end_node(int x, int y, int &start, int &end, Digraph &graph) {
	//Generate start and end vertex numbers
	vector<int> solution;
	start = rand() % (graph.size());
	do {
		solution.clear();
		end = rand() % (graph.size());
		solve_maze(start, end, graph, solution);
	} while (solution.size() < x);
}

int calculate_score(int time, int len_answer, int len_solution, Digraph graph) {
	int efficiency = ((float)len_answer / len_solution) * graph.size();
	int speed = ((float)graph.size() / time) * 100;
	return efficiency * speed;
}