#include "graph.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Graph::Graph(int capacity){
	binaryHeap = heap(capacity);  
	hashtable = hashTable(capacity); 
	vertices.resize(capacity);
	cur_size = 0;
	max_capacity = capacity;
}

void Graph::implementDijkstra(string startIndex) {
	this->initialize(startIndex);

	while (cur_size!=0) { // while there's still unknown vertices
		string v;
		int c;
		void* node;

		// Find the smallest d_val vertex using heap
		binaryHeap.deleteMin(&v, &c, &node);

		cur_size--;

		Node* vertex = static_cast<Node*>(hashtable.getPointer(v));
		vertex->isVisited = true; // unncessary?

		string destName = "";
		//loop through edges of vertex
		for (int i = 0; i < vertex->edges.size(); i++) {
			destName = vertex->edges[i].first.sourceName;
			int c_vw = c + vertex->edges[i].second;

			Node* temp = static_cast<Node*> (hashtable.getPointer(destName)); //d_w

			if (c_vw < temp->d_val) {
				temp->d_val = c_vw;
				binaryHeap.setKey(destName, c_vw);
				temp->path = v;
			}
		}
	}
}

string Graph::writeOutput(string startIndex, int index) {
	string temp = vertices[index].sourceName;
	if (temp == startIndex) {
		return temp + ": 0 [" + temp + "]";
	}
	string res = temp + "]";
	do {
		Node* pPath = static_cast<Node*> (hashtable.getPointer(temp));
		temp = pPath->path;
		if (temp == "") {
			return vertices[index].sourceName + ": NO PATH";
		}
		res = temp + ", " + res;
	} while (temp != startIndex);
	return vertices[index].sourceName + ": " + to_string(vertices[index].d_val) + " [" + res;
}

int Graph::insertNode(const string& vertexName, string& destName, string& cost, void* ptr) {
	if (stoi(cost) < 0) {
		cerr << "Cost can't be less than 0 for this algorithm." << "\n";
		return 2;
	}
	
	if (cur_size > 0.9 * max_capacity) {
		vertices.resize(10 * max_capacity);
		max_capacity *= max_capacity;
	}
	pair<Node, int> temp;

	if (!hashtable.contains(vertexName)) {
		vertices[cur_size].sourceName = vertexName;
		vertices[cur_size].path = "";
		vertices[cur_size].d_val = 1000000;
		vertices[cur_size].isVisited = false;
		vertices[cur_size].ptr = &vertices[cur_size];

		hashtable.insert(vertexName, &vertices[cur_size]);
		binaryHeap.insert(vertexName, 1000000);


		if (!hashtable.contains(destName)) {
			vertices[cur_size + 1].sourceName = destName;
			vertices[cur_size + 1].path = "";
			vertices[cur_size + 1].d_val = 1000000;
			vertices[cur_size + 1].isVisited = false;
			vertices[cur_size + 1].ptr = &vertices[cur_size+1];

			Node copy = vertices[cur_size + 1];
			temp.first = copy;
			temp.second = stoi(cost);
			vertices[cur_size].edges.push_back(temp);

			hashtable.insert(destName, &vertices[cur_size+1]);
			binaryHeap.insert(destName, 1000000);
			cur_size++;
		}
		else {
			Node* destTemp = static_cast<Node*> (hashtable.getPointer(destName));
			temp.first = *destTemp;
			temp.second = stoi(cost);
			vertices[cur_size].edges.push_back(temp);
		}
		cur_size++;
	}
	else{
		Node* sourceTemp = static_cast<Node*> (hashtable.getPointer(vertexName));
		if (!hashtable.contains(destName)) {
			vertices[cur_size].sourceName = destName;
			vertices[cur_size].path = "";
			vertices[cur_size].d_val = 1000000;
			vertices[cur_size].isVisited = false;
			vertices[cur_size].ptr = &vertices[cur_size];
			hashtable.insert(destName, &vertices[cur_size]);
			binaryHeap.insert(destName, 1000000);

			Node copy = vertices[cur_size];
			temp.first = copy;
			temp.second = stoi(cost);
			sourceTemp->edges.push_back(temp);

			cur_size++;

		}
		else {
			Node* destTemp = static_cast<Node*> (hashtable.getPointer(destName));
			temp.first = *destTemp;
			temp.second = stoi(cost);
			sourceTemp->edges.push_back(temp);

		}
	}
	return 0;
}

bool Graph::containsVertex(const string& vertexName) {
	if (hashtable.contains(vertexName) && !vertices.empty()) {
		return true;
	}
	else {
		return false;
	}
}

int Graph::getGraphSize() {
	return cur_size;
}

void Graph::initialize(string startIndex) {
	Node* pnode = static_cast<Node*> (hashtable.getPointer(startIndex));
	pnode->d_val = 0;
	binaryHeap.setKey(pnode->sourceName, 0);
}
