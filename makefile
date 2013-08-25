all: gen_graph
	
clean:
	rm -rf *.o gen_graph
	
gen_graph: gen_graph.o
	g++ gen_graph.o -o gen_graph -O2 -lstdc++
	
gen_graph.o: gen_graph.cpp
	g++ -c gen_graph.cpp -O2 -lstdc++
