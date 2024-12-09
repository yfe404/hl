#include "hl/graph.hpp"
#include "hl/labeling.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <random>
#include <chrono>
#include <omp.h>

using namespace hl;

void generateRandomQueries(std::vector<std::pair<Vertex, Vertex>>& queries, int numberOfRandomQueries, int numberOfVertices) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, numberOfVertices-1); // define the range
	queries.reserve(numberOfRandomQueries);

	for (int i(0); i < numberOfRandomQueries; ++i) queries.push_back(std::make_pair(Vertex(distr(gen)), Vertex(distr(gen))));
}
void usage(char *argv[]) {
	std::cout << "Usage: " << argv[0] << " [-n number of random queries] [-l labeling] graph" << std::endl
			  << "  -n         \tNumber of random queries" << std::endl
			  << "  -l labeling\tFile to read the labeling" << std::endl;
	std::exit(1);
}

int main(int argc, char *argv[]) {
	char *graph_file = NULL;
	char *label_file = NULL;
	int numberOfRandomQueries = 0;
	int argi;
	for (argi = 1; argi < argc; ++argi) {
		if (argv[argi][0] == '-') {
			if (!strcmp("--", argv[argi])) { ++argi; break; }
			else if (!strcmp("-l", argv[argi])) { if (++argi >= argc) usage(argv); label_file = argv[argi]; }
			else if (!strcmp("-n", argv[argi])) { if (++argi >= argc) usage(argv); numberOfRandomQueries = std::stoi(argv[argi]); }
			else usage(argv);
		} else if (graph_file == NULL) graph_file = argv[argi];
		else break;
	}
	if (argi != argc || !graph_file || !label_file) usage(argv);

	Graph g;
	if (!g.read(graph_file)) {
		std::cerr << "Unable to read graph from file " << graph_file << std::endl;
		std::exit(1);
	}
	std::cout << "Graph has " << g.get_n() << " vertices and " << g.get_m() << " arcs" << std::endl;

	Labeling labels(g.get_n());

	if (!labels.read(label_file, g.get_n())) {
		std::cerr << "Unable to read labels from file " << label_file << std::endl;
		std::exit(1);
	}

	// run queries
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	std::vector<std::pair<Vertex, Vertex>> queries;
	generateRandomQueries(queries, numberOfRandomQueries, g.get_n());
	long double totalTime(0);
	for (std::pair<Vertex, Vertex> paar : queries) {
		auto t1 = high_resolution_clock::now();
		labels.query(paar.first, paar.second);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		totalTime += ms_double.count();
	}

	std::cout << "The " << numberOfRandomQueries << " random queries took in total " << totalTime << " [ms] and on average " << (double) (totalTime / numberOfRandomQueries) << " [ms]!\n";
}
