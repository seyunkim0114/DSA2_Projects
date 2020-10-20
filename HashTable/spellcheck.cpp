#include "hash.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

void hashDict(hashTable &);
string processInput(hashTable&, string, int);
void spellCheck(hashTable&);
string tolowerCase(string);

int main() {
	hashTable hashtable(100000);
	clock_t start = clock();
	hashDict(hashtable);
	clock_t finish = clock();
	double tdict = ((double)(finish - start))/CLOCKS_PER_SEC;
	cout << "Total time (in seconds) to load the dictionary: " << tdict << "\n";

	clock_t t3 = clock();
	spellCheck(hashtable);
	clock_t t4 = clock();
	double tspell = ((double)(t4 - t3))/CLOCKS_PER_SEC;
	cout << "Total time (in seconds) to check document: " << tspell << "\n";
	
	return 0;
}

string tolowerCase(string line) {
	for (int i = 0; i < line.length(); i++) {
		line[i] = tolower(line[i]);
	}
	return line;
}

void hashDict(hashTable& hashtable) {
	string dict_file;
	cout << "Enter name of dictionary: ";
	cin >> dict_file;

	ifstream dict(dict_file.c_str());
	if (!dict) {
		cerr << "Error: could not open " << dict_file << "\n";
		exit(1);
	}
	string word;
	// int i;
	while (getline(dict, word)) {
		if(hashtable.insert(tolowerCase(word)) == 2) {  //insert operated on every line of dictionary textfile
			cerr << "Memory allocation failed" << "\n";
			exit(1);
		}
		// i++;
	}
	dict.close();
	return;
}	

string processInput(hashTable& hashtable, string word, int lineNum) {
	int i;
	if (word.length() > 20) {
		return "Long word at line " + to_string(lineNum) + ", starts: " + word.substr(0, 20);
	}

	//Program doesn't spellcheck words that include digits 0-9
	for (i = 0; i < word.length(); i++) {
		if (isdigit(word[i])) {
			return "";
		}
	}

	if (!hashtable.contains(word)) {
		return "Unknown word at line " + to_string(lineNum) + ": " + word;
	}

	return "";

}

void spellCheck(hashTable& hashtable) {
	string input_file;
	cout << "Enter name of input file: ";
	cin >> input_file;

	string output_file;
	cout << "Enter name of output file: ";
	cin >> output_file;

	ifstream input(input_file.c_str());
	ofstream output(output_file.c_str());

	if (!input) {
		cerr << "Error: Could not open " << input_file << "\n";
		exit(1);
	}
	if (!output) {
		cerr << "Error: Could not open " << output_file << "\n";
		exit(1);
	}

	int i = 0;
	int lineNum = 1;
	string word = "";
	string result;
	char c;
	while (input.get(c)) {
		//Checking for valid characters - alphanumeric, dashes, and apostrophes
		//After processing one char, continue to the next char without reaching to the end of the function
		//Used continue; instead of if/else to reduce extra comparison
		if (isdigit(c) || isalpha(c) || c == '-' || c == '\'') {
			word += tolower(c);
			continue;
		}
		//Any character not checked by above if statements are all lineseparators 
		//except for the last character of the file
		if (word != "") {
			result = processInput(hashtable, word, lineNum);
			if (result != "") {
				output << result << "\n";
			}
			word = "";
		}
		if (c == '\n') {
			lineNum++;
		}
		//Skip char when it's a line separator and no word has been formulated
		continue;
	}
	input.close();
	output.close();
}

