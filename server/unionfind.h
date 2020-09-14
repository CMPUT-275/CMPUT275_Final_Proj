/*
	Implementation of the union-find data structure
	for the int type (i.e. the set we are partitioning
  is a set of integers).
*/

#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <unordered_map>

class UnionFind {
public:
	// A constructor is not necessary, the parent and rank
	// mappings will just be empty upon initialization.

	// Adds an item to the data structure, if it was there already
	// then this does nothing (it does not change the partition).
	// If not, it is added as a singleton part {v} in the partition.
	void add_item(int v);

	// Find the representative for the set containing u.
	int find(int u);

	// Merge the sets, returns true iff the merge was successful
	// (i.e. they were not in the same set already).
	// Cannot use the function name "union" :(  It is a keyword in c++.
	bool merge(int u, int v);

private:
	std::unordered_map<int, int> parent, rank;
};

#endif
