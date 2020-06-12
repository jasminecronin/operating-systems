/********************************************
 * Last Name: Roebuck
 * First Name: Jasmine
 * Course: Operating Systems
 * Tutorial: 3
 * Assignment: 3
 * Question: Q4
 *
 * File name: deadlock.cpp
 * *****************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

struct node {
	vector<string> adjacencylist;
	vector<string> visitedEdges;
	bool visited = false;
};

void printCycle(unordered_set<string> visited);

void detectCycles(unordered_map<string, node> &graph) {
	stack<node> path;
	unordered_set<string> visited;
	bool cycle = false;

	// For each node in the graph
	for (auto& n : graph) {
		// Only process unvisited nodes
		if (n.second.visited) continue;
		// Set all edges as unmarked
		for (auto& n : graph) {
			n.second.visitedEdges = n.second.adjacencylist;
		}
		
		// Clear the stack
		while(!path.empty()) path.pop();
		visited.clear(); // Clear the set of visited nodes

		path.push(n.second);
	        n.second.visited = true;
		visited.insert(n.first); // Add to the list of visited nodes
		while(!path.empty() && !cycle) {
			node s = path.top();
			path.pop();

			while(s.visitedEdges.size() > 0) {
				string nextNode = s.visitedEdges.back();
				s.visitedEdges.pop_back();

				if (graph.at(nextNode).visited) {
					cycle = true;
					break;
				}
				path.push(graph.at(nextNode));
				visited.insert(nextNode);
				graph.at(nextNode).visited = true;
			}
		}
		if (cycle) {
		       printCycle(visited);
		       cycle = false;
		       break;
		}
	}
}

void printCycle(unordered_set<string> visited) {
	
	vector<int> cycle;
	for (auto key : visited) {
		if (key.back() == 'R') continue;
		int i = stoi(key);
		cycle.push_back(i);
	}

	sort(cycle.begin(), cycle.end());
	cout << "Deadlocked processes: ";
	if (cycle.size() < 2) cout << "none";
	else {
		for (auto n : cycle) cout << n << " ";
	}
	cout << endl;
}

int main(int argc, char ** argv) {

	unordered_map<string, node> graph;

	while(1) {
		graph.clear();
		while(1) {
			string line;
			getline(cin, line);
			if (cin.eof() || line[0] == '#') {
				detectCycles(graph);
				break;
			}
			else {
				vector<string> words;
				stringstream ss(line);
				string item;
				while (getline(ss, item, ' ')) {
					*(back_inserter(words)++) = item;
				}
				words[2] += "R"; // mark the resource key as such

				if (words[1].compare("->") == 0) {
					if (graph.count(words[0]) == 0) {
						node n;
						graph.insert({words[0], n});		
					}
					if (graph.count(words[2]) == 0) {
						node n;
						graph.insert({words[0], n});
					}
					graph.at(words[0]).adjacencylist.push_back(words[2]);
				}
				else {
					if (graph.count(words[2]) == 0) {
						node n;
						graph.insert({words[2], n});
					}
					if (graph.count(words[0]) == 0) {
						node n;
						graph.insert({words[0], n});
					}
					graph.at(words[2]).adjacencylist.push_back(words[0]);
				}
			}
		}
		if (cin.eof()) break;
	}

}
