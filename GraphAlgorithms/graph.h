#pragma once
#ifndef _GRAPH_H
#define _GRAPH_H
#include <string>
#include <list>
#include <vector>
#include "heap.h"
#include "hash.h"
using namespace std;

class Graph {
public:
	Graph(int capacity);

	void implementDijkstra(string startIndex);
	
	bool containsVertex(const string& vertexName);

	int insertNode(const string& vertexName, string& destName, string& cost, void* ptr = nullptr);
	
	string writeOutput(string startIndex, int index);

	int getGraphSize();

	int cur_size;

private:
	class Node {
	public:
		string sourceName{ "" };
		string path{ "" };
		void* ptr{ nullptr };
		int d_val{ 1000000 };
		bool isVisited{ false };
		vector<pair<Node, int>> edges; //adjacency list 
		Node() = default;
	};

	vector<Node> vertices;
	heap binaryHeap;

	hashTable hashtable;

	void initialize(string startIndex);

	int max_capacity;
};

#endif //_GRAPH_H
