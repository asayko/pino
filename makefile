all: gen_graph 2edge_conn_flow
	
clean:
	rm -rf *.o gen_graph 2edge_conn_flow
	
gen_graph: gen_graph.o
	g++ gen_graph.o -o gen_graph -O2 
	
gen_graph.o: gen_graph.cpp
	g++ -c gen_graph.cpp -O2

2edge_conn_flow: 2edge_conn_flow.o
	g++ 2edge_conn_flow.o -o 2edge_conn_flow -O2
	
2edge_conn_flow.o: 2edge_conn_flow.cpp
	g++ -c 2edge_conn_flow.cpp -O2
	