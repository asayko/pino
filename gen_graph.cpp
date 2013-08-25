/*
 * gen_graph.cpp
 *
 *  Created on: 25.08.2013
 *      Author: asayko
 *
 *      Creates directed strongly connected graph based on ear decomposition idea.
 */

#include <iostream>
#include <set>
#include <utility>
#include <cstdlib>

using namespace std;

typedef set<pair<size_t, size_t> > TEdges;

void add_edge(TEdges & edges, size_t a, size_t b) {
	pair<size_t, size_t> t = make_pair(a, b);

	if (edges.find(t) == edges.end()) {
		cout << a << " " << b << endl;
		edges.insert(t);
	} else {
		cerr << "Generated duplicate edge from " << a << " to " << b << "." << endl;
	}
}

pair<size_t, size_t> pick_two_vertecies_in_graph(size_t graph_size) {
	size_t a = rand() % graph_size;
	size_t b = rand() % graph_size;
	while (b == a) {
		b = rand() % graph_size;
	}
	pair<size_t, size_t> res(a, b);
	return res;
}

int main(int argc, char ** argv) {

	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " N [sparsity_koef=2]" << endl;
		exit(0);
	}

	size_t N = atoi(argv[1]);
	cerr << "Generating directed strongly connected graph of size " << N << "." << endl;
	cout << N << endl;

	size_t termination_sparsity_koef = 3 * N;
	if (argc > 2) {
		float t = atof(argv[2]);
		termination_sparsity_koef = t * N;
	}

	size_t current_graph_size = rand() % (N - 1) + 2;
	cerr << "Initial cycle size is taken for " << current_graph_size << "." << endl;

	TEdges edges;
	cerr << "Generating initial cycle." << endl;
	for (size_t i = 0; i < current_graph_size - 1; ++i) {
		add_edge(edges, i, i + 1);
	}
	add_edge(edges, current_graph_size - 1, 0);

	cerr << "Adding eras." << endl;

	bool term_criteria = false;
	while (!term_criteria) {
		pair<size_t, size_t> t = pick_two_vertecies_in_graph(current_graph_size);
		size_t from_v = t.first;
		size_t to_v = t.second;

		size_t vertecies_not_in_graph = N - current_graph_size;
		size_t take_new_vertecies = 0;
		if (vertecies_not_in_graph > 0) {
			take_new_vertecies = rand() % vertecies_not_in_graph + 1;
		}

		for (size_t new_v = current_graph_size; new_v < current_graph_size + take_new_vertecies; ++new_v) {
			add_edge(edges, from_v, new_v);
			from_v = new_v;
		}
		add_edge(edges, from_v, to_v);
		current_graph_size += take_new_vertecies;
		term_criteria = current_graph_size == N && rand() % termination_sparsity_koef == 0;
	}

	cerr << "Strongly connected graph from " << N << " vertices and " << edges.size() << " edges is generated." << endl;

	return 0;
}



