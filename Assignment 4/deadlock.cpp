/********************************************
 * Last Name: Roebuck
 * First Name: Jasmine
 * Student ID: 30037334
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

	//cout << "detectCycles entered" << endl;

	// For each node in the graph
	for (auto& n : graph) {
		//cout << "Starting from node: " << n.first << endl;
		//cout << "first for loop entered" << endl;
		// Only process unvisited nodes
		if (n.second.visited) continue;
		// Set all edges as unmarked
		for (auto& n : graph) {
			//cout << "clearing all marked edges" << endl;
			n.second.visitedEdges = n.second.adjacencylist;
		}
		
		// Clear the stack
		while(!path.empty()) path.pop();
		visited.clear(); // Clear the set of visited nodes

		path.push(n.second);
	        n.second.visited = true;	

		//if (visited.count(n.first) != 0) { // If we've seen this node before, we have a cycle
		//	printCycle(visited);
		//	cout << "Cycle detected when first node added" << endl;
		//	break;
		//}
		//else {
			visited.insert(n.first); // Add to the list of visited nodes
			while(!path.empty() && !cycle) {
				//cout << "DFS while loop entered" << endl;
				node s = path.top();
				path.pop();

				while(s.visitedEdges.size() > 0) {
					//cout << "pushing unvisited nodes to the stack" << endl;
					string nextNode = s.visitedEdges.back();
					s.visitedEdges.pop_back();

					if (graph.at(nextNode).visited) {
						//cout << "cycle if check made" << endl;
						cycle = true;
						//printCycle(visited);
						//cout << "Cycle detected while checking unmarked edges" << endl;
						break;
					}
					//cout << "nextNode = " << nextNode << endl;
					//for (auto it = graph.begin(); it != graph.end(); it++) cout << it->first << endl;
					path.push(graph.at(nextNode));
					//cout << "accessed graph node, pushed to path" << endl;
					visited.insert(nextNode);
					//cout << "node added to list of involved nodes" << endl;
					graph.at(nextNode).visited = true;
					//cout << "node marked as visited" << endl;
				}
				//if (cycle) {
				//	printCycle(visited);
				//	cycle = false;
				//	break;
				//}
			}
			if (cycle) {
			       printCycle(visited);
			       cycle = false;
			       break;
			}
		//}
	}
}

void printCycle(unordered_set<string> visited) {
	// for every string in this list
	// if it ends in an R, skip
	// else, add as int to a sortable list
	// sort the list
	// do the printing
	//cout << "Cycle detected" << endl;
	//cout << "involved nodes ";
	//for (auto c : visited) cout << c << " ";
	//cout << endl;

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
				//for (auto it = graph.begin(); it != graph.end(); it++) cout << it->first << endl;
				detectCycles(graph);
				//for (auto& x: graph) {
				//	cout << x.first << ": " << x.second.adjacencylist[0] << endl;
				//}
				break;
			}
			else {
				vector<string> words;
				stringstream ss(line);
				string item;
				while (getline(ss, item, ' ')) {
					*(back_inserter(words)++) = item;
					//cout << item << endl;
				}
				//cout << line << endl;
				words[2] += "R"; // mark the resource key as such

				if (words[1].compare("->") == 0) {
					if (graph.count(words[0]) == 0) {
						//cout << "Added process key: " << words[0] << endl;
						node n;
						graph.insert({words[0], n});		
					}
					if (graph.count(words[2]) == 0) {
						//cout << "Added resource key: " << words[2] << endl;
						node n;
						graph.insert({words[0], n});
					}
					graph.at(words[0]).adjacencylist.push_back(words[2]);
				}
				else {
					if (graph.count(words[2]) == 0) {
						//cout << "Added resource key: " << words[2] << endl;
						node n;
						graph.insert({words[2], n});
					}
					if (graph.count(words[0]) == 0) {
						//cout << "Added process key: " << words[0] << endl;
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
