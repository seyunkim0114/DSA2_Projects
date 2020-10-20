#include "heap.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// left child of a node in pos i is 2*i
// right child is 2*i+1
heap::heap(int capacity) {
	hash_table = hashTable(2 * capacity);
	current_size = 0;
	cap = capacity;
	nodes.resize(capacity + 1);
	//Maxcapacity = capacity;
}

// Inserts a node with the specified id string, key,
// and optionally a pointer.  They key is used to
// determine the final position of the new node.
//
// Returns:
//   0 on success
//   1 if the heap is already filled to capacity
//   2 if a node with the given id already exists (but the heap
//     is not filled to capacity)
//
int heap::insert(const string& id, int key, void* pv) {
	// heap filled to capacity
	if (current_size == cap) {
		return 1;
	}
	else {
		if (hash_table.contains(id)) {
			return 2;
		}
		else {
			current_size++;
			nodes[current_size].key = key;
			nodes[current_size].id = id;
			nodes[current_size].ptr = pv;
			hash_table.insert(id, pv);
			percolateUp(current_size);
			return 0;
		}
	}
}

// setKey - set the key of the specified node to the specified value
//
// I have decided that the class should provide this member function
// instead of two separate increaseKey and decreaseKey functions.
//
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
// NEEDS REVISION
int heap::setKey(const string& id, int key) {
	if (!hash_table.contains(id)) {
		return 1;
	}
	else {
		Node* pNode = static_cast<Node*>(hash_table.getPointer(id));
		int ind = getPos(pNode);
		pNode->key = key;
		if (key < nodes[ind / 2].key) {
			percolateUp(ind);
		}
		else if (key > nodes[ind * 2].key || key > nodes[ind*2+1].key) {
			percolateDown(ind);
		}
		return 0;
	}
}

// deleteMin - return the data associated with the smallest key
	//             and delete that node from the binary heap
	//
	// If pId is supplied (i.e., it is not nullptr), write to that address
	// the id of the node being deleted. If pKey is supplied, write to
	// that address the key of the node being deleted. If ppData is
	// supplied, write to that address the associated void pointer.
	//
	// Returns:
	//   0 on success
	//   1 if the heap is empty
	//
int heap::deleteMin(string* pId, int* pKey, void* ppData) {
	if (current_size == 0) {
		return 1;
	}
	else {
		if (pId != nullptr) {
			*pId = nodes[1].id;
		}
		if (pKey != nullptr) {
			*pKey = nodes[1].key;
		}
		if (ppData != nullptr) {
			*(static_cast<void**> (ppData)) = nodes[1].ptr;
		}
		hash_table.remove(nodes[1].id);
		nodes[1] = nodes[current_size];
		--current_size;
		percolateDown(1);
		return 0;
	}
}

// remove - delete the node with the specified id from the binary heap
//
// If pKey is supplied, write to that address the key of the node
// being deleted. If ppData is supplied, write to that address the
// associated void pointer.
//
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
//
//NEEDS REVISION
int heap::remove(const string& id, int* pKey, void* ppData) {
	if (!hash_table.contains(id)) {
		return 1;
	}
	else {
		Node* pNode = static_cast<Node*>(hash_table.getPointer(id));
		if (pKey != nullptr) {
			*pKey = pNode->key;
		}
		if (ppData != nullptr) {
			*(static_cast<void**>(ppData)) = pNode->ptr;
		}
		setKey(id, nodes[1].key - 1);
		deleteMin();
		return 0;
	}
}

void heap::percolateUp(int pos) {
	Node temp = nodes[pos];
	while (pos > 1) {
		if (temp.key < nodes[pos/2].key) {
			nodes[pos] = nodes[pos / 2];
			hash_table.setPointer(nodes[pos].id, &nodes[pos]);
			pos = pos / 2;
		}
		else {
			break;
		}
	}
	nodes[pos] = temp;
	hash_table.setPointer(nodes[pos].id, &nodes[pos]);
}

//NEEDS REVIEW
//pos = index 1
void heap::percolateDown(int pos) {
	Node temp = nodes[pos];
	while (2*pos < current_size+1) {
		if (temp.key > nodes[2 * pos].key || temp.key > nodes[2 * pos + 1].key) {
			if (nodes[2 * pos].key < nodes[2 * pos + 1].key) { //CHECK IF VALID LOCATION NEEDED
				nodes[pos] = nodes[2 * pos];
				hash_table.setPointer(nodes[pos].id, &nodes[pos]);
				pos = 2 * pos;
			}
			else if (nodes[2 * pos].key >= nodes[2 * pos + 1].key) {
				nodes[pos] = nodes[2 * pos + 1];
				hash_table.setPointer(nodes[pos].id, &nodes[pos]);
				pos = 2 * pos + 1;
			}
		}
		
		else {
			break;
		}
	}
	nodes[pos] = temp;
	hash_table.setPointer(nodes[pos].id, &nodes[pos]);
}

int heap::getPos(Node* pnode) {
	int pos = pnode - &nodes[0];
	return pos;
}