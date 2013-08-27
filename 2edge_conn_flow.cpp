/*
 * graph_to_network.cpp
 *
 *  Created on: 25.08.2013
 *      Author: asayko
 */

#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef set<pair<size_t, size_t> > TEdges;

typedef adjacency_list_traits < vecS, vecS, directedS > Traits;

typedef adjacency_list < vecS, vecS, directedS,
		no_property,
		property<edge_capacity_t, long,
			property<edge_residual_capacity_t, long,
				property<edge_reverse_t, Traits::edge_descriptor> > >
> Graph;

int main(int argc, char ** argv) {

	size_t N;
	TEdges edges;
	cin >> N;
	size_t current_network_size = N;

	cerr << "Converting graph of  size " << N << " to network." << endl;

	while (!cin.eof()) {
		size_t a;
		size_t b;
		cin >> a;
		cin >> b;
		pair<size_t, size_t> edge = make_pair(a, b);

		if (edges.find(make_pair(b, a)) == edges.end()) {
			cerr << "No parallel edge for (" << a << ", " << b << ")." << endl;
			cerr << "Inserting  (" << a << ", " << b << ")." << endl;
			edges.insert(edge);
		} else {
			cerr << "There is a parallel edge for (" << a << ", " << b << ")." << endl;
			pair<size_t, size_t> edge_first = make_pair(a, current_network_size);
			pair<size_t, size_t> edge_second = make_pair(current_network_size, b);
			cerr << "Inserting  (" << a << ", " << current_network_size << ")." << endl;
			edges.insert(edge_first);
			cerr << "Inserting  (" << current_network_size << ", " << b << ")." << endl;
			edges.insert(edge_second);
			++current_network_size;
		}
	}

	cerr << "The final network's size is " << current_network_size << "." << endl;


	long flow = 0;
	Graph g(current_network_size);

	property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
	property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);
	property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);

	for (TEdges::const_iterator it = edges.begin(); it != edges.end(); ++it) {
		cout << it->first << " " << it->second << endl;
		Traits::edge_descriptor e1 = add_edge(it->first, it->second, g).first;
		Traits::edge_descriptor e1r = add_edge(it->second, it->first, g).first;
		put(edge_capacity, g, e1, 1);
		put(edge_capacity, g, e1r, 0);
		rev[e1] = e1r;
		rev[e1r] = e1;
	}

	for (size_t i = 1; i < N; ++i) {
		cerr << "Checking the flow from 0 to " << i << ". " << endl;
		flow = push_relabel_max_flow(g, 0, i);
		assert(flow >= 1);
		cerr << "The flow equals to " << flow << endl;

		if (flow > 1) {
			cerr << "The vertex " << i << "is 2-edge connected with 0." << endl;
			cout << i << endl;
		}
	}

	return 0;
}
