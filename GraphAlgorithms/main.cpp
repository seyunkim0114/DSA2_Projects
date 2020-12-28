#include <iostream>
#include <string>
#include <fstream>
#include "graph.h"
#include <ctime>
#include <cmath>
#include <chrono>

using namespace std;

int main() {
	int capacity = 1000000;
	Graph graph(capacity);

	string filename;
	string inputline;
	cout << "Enter name of graph file: ";
	cin >> filename;
	
	ifstream input(filename);
	
	if (!input) {
		cerr << "Error: Cannot open file." << "\n";
		exit(1);
	}
	
	else {
		string vertex, dest, cost;
		int index, index2;
		while (getline(input, inputline)) {
			index = inputline.find(" ");
			vertex = inputline.substr(0, index);
			index2 = inputline.find(' ', index + 1);
			dest = inputline.substr(index + 1, index2 - (index + 1));
			cost = inputline.substr(index2 + 1, inputline.length() - (index2+1));

			graph.insertNode(vertex, dest, cost);
		}
	}
	int verticeSize = graph.getGraphSize();

	string startingVertex;

	do {
		cout << "Enter a valid vertex id for the starting vertex: ";
		cin >> startingVertex;
	} while (!graph.containsVertex(startingVertex));
	
	string outputFile;
	cout << "Enter name of output file: ";
	cin >> outputFile;

	ofstream output(outputFile);

	if (!output) {
		cerr << "Error: cannot open " << outputFile << "\n";
		exit(1);
	}

	else {
		// Meaure time taken to implement Dijkstra algorithm
		chrono::steady_clock::time_point start = chrono::steady_clock::now();
		graph.implementDijkstra(startingVertex);
		chrono::steady_clock::time_point finish = chrono::steady_clock::now();

		double graphTime = (chrono::duration_cast<chrono::microseconds>(finish - start).count()) / 1000000.0;
		cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << graphTime << "\n";
	}
	for (int i = 0; i < verticeSize; i++) {
		output << graph.writeOutput(startingVertex, i) << "\n";
	}
	//cout << "Saved as " << outputFile << "\n";
	
	input.close();
	output.close();
}
