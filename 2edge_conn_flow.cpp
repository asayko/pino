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
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/preflow_push_max_flow.hpp>

using namespace std;
using namespace boost;

typedef set<pair<size_t, size_t> > TEdges;

typedef int EdgeWeightType;

typedef adjacency_list_traits < vecS, vecS, directedS > Traits;

typedef adjacency_list < vecS, vecS, directedS,
  property < vertex_name_t, std::string,
    property < vertex_index_t, long,
      property < vertex_color_t, boost::default_color_type,
        property < vertex_distance_t, long,
          property < vertex_predecessor_t, Traits::edge_descriptor > > > > >,

  property < edge_capacity_t, EdgeWeightType,
    property < edge_residual_capacity_t, EdgeWeightType,
      property < edge_reverse_t, Traits::edge_descriptor > > > > Graph;

Traits::edge_descriptor AddEdge(Traits::vertex_descriptor &v1,
                                Traits::vertex_descriptor &v2,
                                property_map < Graph, edge_reverse_t >::type &rev,
                                EdgeWeightType capacity,
                                Graph &g);

Traits::edge_descriptor AddEdge(Traits::vertex_descriptor &v1,
								Traits::vertex_descriptor &v2,
								property_map < Graph, edge_reverse_t >::type &rev,
								EdgeWeightType capacity,
								Graph &g) {
  Traits::edge_descriptor e1 = add_edge(v1, v2, g).first;
  Traits::edge_descriptor e2 = add_edge(v2, v1, g).first;
  put(edge_capacity, g, e1, capacity);
  put(edge_capacity, g, e2, capacity);

  rev[e1] = e2;
  rev[e2] = e1;
}


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


	Graph g(current_network_size);
	property_map < Graph, edge_reverse_t >::type rev = get(edge_reverse, g);

	for (TEdges::const_iterator it = edges.begin(); it != edges.end(); ++it) {
		cout << it->first << " " << it->second << endl;
		AddEdge(it->first, it->second, rev, 1);
	}

	for (size_t i = 1; i < N; ++i) {
		cerr << "Checking the flow from 0 to " << i << ". " << endl;
		EdgeWeightType flow = boykov_kolmogorov_max_flow(g, 0, i); // a list of sources will be returned in s, and a list of sinks will be returned in t
		//EdgeWeightType flow = push_relabel_max_flow(g, 0, i); // a list of sources will be returned in s, and a list of sinks will be returned in t
		//EdgeWeightType flow = edmunds_karp_max_flow(g, 0, i); // a list of sources will be returned in s, and a list of sinks will be returned in t
		assert(flow >= 1);
		cerr << "The flow equals to " << flow << endl;

		if (flow > 1) {
			cerr << "The vertex " << i << "is 2-edge connected with 0." << endl;
			cout << i << endl;
		}
	}

	return 0;
}
