#include "hash.h"
#include <iostream>

using namespace std;

hashTable::hashTable(int size) {
	capacity = getPrime(size);
	data.resize(capacity);
	for (int i = 0; i < data.size(); i++) {
		data[i].isDeleted = false;
		data[i].isOccupied = false;
	}
	filled = 0;
}

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
int hashTable::insert(const string& key, void* pv) {
	int val = hash(key);
	while (data[val].isOccupied == true) {
		if (data[val].key == key) {
			if (data[val].isDeleted == false) {
				return 1;
			}
			else {
				data[val].isDeleted = false;
				data[val].pv = pv;
				// filled++;
				return 0;
			}
		}
		else {
			val++;
			if (val >= capacity) {
				val = val % capacity;
			}
		}
	}
	data[val].key = key;
	data[val].isOccupied = true;
	data[val].isDeleted = false;
	data[val].pv = pv;
	filled++;
	if (filled > capacity * 0.5) {
		if (rehash()) {
			return 0;
		}
		else {
			return 2;
		}
	}
	return 0;
}

// Check if the specified key is in the hash table.
// If so, return true; otherwise, return false.
bool hashTable::contains(const string& key) {
	if (findPos(key) != -1) {
		return true;
	}
	else {
		return false;
	}
}


int hashTable::hash(const string& key) {
	int val = 0;
	for (int i = 0; i < key.length(); i++) {
		val = 37 * val + key[i];
	}
	val = val % capacity;
	if (val < 0) {
		val = val + capacity;
	}
	return val;
}

// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const string& key) {
	int val = hash(key);

	while (data[val].isOccupied == true) {
		if (data[val].key == key) {
			if (data[val].isDeleted == false) {
				return val;
			}
			else {
				return -1;
			}
		}
		else {
			val++;
			if (val > capacity) {
				val = val % capacity;
			}
		}
	}
	return -1;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash() {
	vector<hashItem> temp = data;

	for (int i = 0; i < capacity; i++) {
		data[i].key = "";
		data[i].isDeleted = false;
		data[i].isOccupied = false;
		data[i].pv = NULL;
	}
	filled = 0;

	capacity = getPrime(capacity);

	try {
		data.resize(capacity);
	}
	catch (exception& e) {
		cerr << "Memory allocation failed" << "\n";
		return false;
	}

	for (int i = 0; i < temp.size(); i++) {
		if (temp[i].isDeleted == false && temp[i].isOccupied == true) {
			insert(temp[i].key, NULL);
		}
	}
	return true;
}

// Return a prime number at least as large as size.
// Uses a precomputed sequence of selected prime numbers.
// Prime number list source: https://studylib.net/doc/7932288/good-hash-table-primes
unsigned int hashTable::getPrime(int size) {
	long long int primeNums[] = { 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457 };
	for (int i = 0; i < 20; i++) {
		if (primeNums[i] > size) {
			return primeNums[i];
		}
	}
	return 610612741;
}

// Get the pointer associated with the specified key.
// If the key does not exist in the hash table, return nullptr.
// If an optional pointer to a bool is provided,
// set the bool to true if the key is in the hash table,
// and set the bool to false otherwise.
void* hashTable::getPointer(const string& key, bool* b) {
	int pos = findPos(key);

	if (pos == -1) {
		if (b != nullptr) {
			*b = false;
		}
		return nullptr;
	}
	else {
		if (b != nullptr) {
			*b = true;
		}
		return data[pos].pv;
	}
}

// Set the pointer associated with the specified key.
// Returns 0 on success,
// 1 if the key does not exist in the hash table.
int hashTable::setPointer(const string& key, void* sptr) {
	int pos = findPos(key);

	if (pos == -1) {
		return 1;
	}
	else {
		data[pos].pv = sptr;
		return 0;
	}
}

// Delete the item with the specified key.
// Returns true on success,
// false if the specified key is not in the hash table.
bool hashTable::remove(const string& key) {
	int pos = findPos(key);
	
	if (pos == -1) {
		return false;
	}
	else {
		data[pos].isDeleted = true;
		filled = filled - 1;
		return true;
	}
}
