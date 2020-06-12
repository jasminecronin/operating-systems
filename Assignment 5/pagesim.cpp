/**********************************************************
 * Last Name:	Roebuck
 * First Name:	Jasmine
 * Student ID:	30037334
 * Course:	Operating Systems
 * Tutorial:	T03
 * Assignment:	5
 * Question:	Q6
 *
 * File Name:	pagesim.cpp
 * *******************************************************/

#include <iostream>
#include <vector>

using namespace std;

vector<int> referenceString;
vector<int> pageTable;
int pageFaults = 0;

void optimalReplacement();
void lruReplacement();
void clockReplacement();

int main(int argc, char ** argv) {
	int availablePages;
	availablePages = atoi(argv[1]);
	// cout << availablePages << endl;
	
	//vector<int> referenceString;
	//cout << "Reference String created" << endl;
	//int pageTable[availablePages] = {0};
	//cout << "Page table created" << endl;
	for (int i = 0; i < availablePages; i++) {
		pageTable.push_back(-1);
	}
	
	while (1) {
		int num;
		if (1 != scanf("%ld", & num)) break;
		//cout << "Attempting to push back" << endl;
		referenceString.push_back(num);
		//cout << "Push back successful" << endl;
	}

	//for (int n : referenceString) {
		//cout << n << " ";
	//}
	//cout << endl;
	
	optimalReplacement();
	//cout << "Optimal finished" << endl;
	lruReplacement();
	//cout << "LRU finished" << endl;
	clockReplacement();
	//cout << "Clock finished" << endl;
}

void optimalReplacement() {
	//cout << "Optimal called" << endl;
	//pageTable[0] = 1;
	//cout << "pTable edited" << endl;
	//cout << pageTable[0] << endl;
	
	pageFaults = 0;
	bool hit = false;
	for (int i = 0; i < pageTable.size(); i++) pageTable[i] = -1;

	for (int i = 0; i < referenceString.size(); i++) {
		for (int j = 0; j < pageTable.size(); j++) {
			if (pageTable[j] == referenceString[i]) {
				hit = true;
				break;
			}
		}
		if (hit) {
			hit = false;
			continue;
		}
		else { // page fault
			pageFaults++;
			vector<int> candidates = pageTable;
			for (int k = i + 1; k < referenceString.size(); k++) {
				if (candidates.size() <= 1) break;
				for (int l = 0; l < candidates.size(); l++) {
					if (candidates[l] == -1) {
						candidates[0] = -1;
						break;
					}
					if (candidates[l] == referenceString[k]) { // we will need this soon, remove candidacy
						candidates.erase(candidates.begin() + l);
						break;
					}
				}
			}
			for (int k = 0; k < pageTable.size(); k++) {
				if (pageTable[k] == candidates[0]) {
					pageTable[k] = referenceString[i];
					break;
				}
			}
			//for (int frame : pageTable) cout << frame << " ";
			//cout << endl;
		}
	}

	cout << "Optimal:" << endl;
	cout << "  - frames: ";
	for (int frame : pageTable) {
		cout << frame << " ";
	}
	cout << endl;
	cout << "  - page faults: " << pageFaults << endl;
}

void lruReplacement() {
	//cout << "LRU called" << endl;
	//cout << pageTable[0] << endl;
	
	pageFaults = 0;
	bool hit = false;
	for (int i = 0; i < pageTable.size(); i++) pageTable[i] = -1;

	for (int i = 0; i < referenceString.size(); i++) {
		for (int j = 0; j < pageTable.size(); j++) {
			if (pageTable[j] == referenceString[i]) {
				hit = true;
				break;
			}
		}
		if (hit) {
			hit = false;
			continue;
		}
		else {
			pageFaults++;
			vector<int> candidates = pageTable;
			for (int k = i; k > 0; k--) {
				if (candidates.size() <= 1) break;
				for (int l = 0; l < candidates.size(); l++) {
					if (candidates[l] == -1) {
						candidates[0] = -1;
						break;
					}
					if (candidates[l] == referenceString[k]) {
						candidates.erase(candidates.begin() + l);
						break;
					}
				}
			}
			for (int k = 0; k < pageTable.size(); k++) {
				if (pageTable[k] == candidates[0]) {
					pageTable[k] = referenceString[i];
					break;
				}
			}
			//for (int frame : pageTable) cout << frame << " ";
			//cout << endl;
		}
	}

	cout << "LRU:" << endl;
	cout << "  - frames: ";
	for (int frame : pageTable) {
		cout << frame << " ";
	}
	cout << endl;
	cout << "  - page faults: " << pageFaults << endl;
}


void clockReplacement() {
	//cout << "Clock called" << endl;
	//cout << pageTable[0] << endl;
	
	pageFaults = 0;
	bool hit = false;
	for (int i = 0; i < pageTable.size(); i++) pageTable[i] = -1;

	vector<int> refs;
	for (int i = 0; i < pageTable.size(); i++) {
		refs.push_back(0);
	}
	int hand = 0;

	for (int i = 0; i < referenceString.size(); i++) {
		for (int j = 0; j < pageTable.size(); j++) {
			if (pageTable[j] == referenceString[i]) {
				hit = true;
				refs[j] = 1;
				break;
			}
		}
		if (hit) {
			hit = false;
			continue;
		}
		else {
			pageFaults++;
			while (refs[hand] == 1) {
				refs[hand] = false;
				hand = (hand + 1) % pageTable.size();
			}
			pageTable[hand] = referenceString[i];
			refs[hand] = 1;
			hand = (hand + 1) % pageTable.size();
		}
	}

	cout << "Clock:" << endl;
	cout << "  - frames: ";
	for (int frame : pageTable) {
		cout << frame << " ";
	}
	cout << endl;
	cout << "  - page faults: " << pageFaults << endl;
}
