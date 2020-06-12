/************************************
 * Last Name: Roebuck
 * First Name: Jasmine
 * Course: Operating Systems
 * Tutorial: 3
 * Assignment 3
 * Question: Q7
 *
 * File name: scheduler.cpp
 * *********************************/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

struct process {
	string name;
	int id;
	int arrival;
	int burst;
	int length;
	int start;
	int end = 0;
	bool started = false;
	bool added = false;
};

int totaltime = 0; // Add up total time to run here, run loop until this hits zero?
vector <process> processes;
queue <process> readyqueue;
vector <process> sjqueue;

void roundrobin(int s);
void shortestjobfirst();
void printHeader();
void printStatuses(int active, int time);
void printTrailer();

void roundrobin(int s) { 
	printHeader();
	int next = 0;
	int slice = s;
	int active;
	for (int time = 0; time < totaltime; time++) {
		// Check if any process has arrived
		// cout << "next: " << next << endl;
		if (next < processes.size() && time == processes.at(next).arrival) {
			readyqueue.push(processes.at(next));
			processes.at(next).added = true;
			next++;
		}
		if (readyqueue.empty()) {
			printf("%3d\t\n", time);
			continue;
		}
		
		active = readyqueue.front().id; // Get first item id from queue
		//cout << "active: " << active << " time left: " << processes.at(active).burst << endl;
		if (!processes.at(active).started) {
			processes.at(active).started = true;
			processes.at(active).start = time;
		}

		printStatuses(active, time);
		
		slice--;
		processes.at(active).burst--;
		if (processes.at(active).burst <= 0) {
			processes.at(active).end = time + 1;
			readyqueue.pop();
			slice = s;
		}
		else if (slice <= 0) {
			readyqueue.push(readyqueue.front());
			readyqueue.pop();
			slice = s;
		}

		//active = readyqueue.front().id;
		//printStatuses(active, time);
	}
	printTrailer();
}

void shortestjobfirst() {
	printHeader();
	int active = 0;

	int next = 0;

	for (int time = 0; time < totaltime; time++) {
		// Check if any process has arrived
		if (next < processes.size() && time == processes.at(next).arrival) {
			if (sjqueue.size() > 1) {
				for (int i = 1; i < sjqueue.size(); i++) {
					if (processes.at(next).length < sjqueue[i].length) {
						sjqueue.insert(sjqueue.begin() + i, processes.at(next));
						processes.at(next).added = true;
						break;
					}
				}
			}
			if (!processes.at(next).added) {
				sjqueue.push_back(processes.at(next));
				processes.at(next).added = true;
			}
			next++;
		}
		if (sjqueue.size() == 0) {
			printf("%3d\t\n", time);
			continue;
		}

		active = sjqueue[0].id;
		if (!processes.at(active).started) {
			processes.at(active).started = true;
			processes.at(active).start = time;
		}
		printStatuses(active, time);
		processes.at(active).burst--;
		if (processes.at(active).burst <= 0) {
			processes.at(active).end = time + 1;
			sjqueue.erase(sjqueue.begin());
		}
	}
	printTrailer();
}

void printHeader() {
	cout << "Time\t";
	for (int i = 0; i < processes.size(); i++) {
		cout << processes[i].name + "\t";
	}
	cout << endl;
	for (int i = 0; i <= processes.size(); i++) {
		cout << "--------";
	}
	cout << endl;
}

void printStatuses(int active, int time) {
	printf("%3d\t", time);
	for (process p : processes) {
		if (!p.added || p.end != 0) {
			cout << "\t";
		}
		else if (p.id == active) {
			cout << ".\t";
		}
		else {
			cout << "+\t";
		}
	}
	cout << endl;
}

void printTrailer() {
	int total = 0;
	for (int i = 0; i <= processes.size(); i++) {
		cout << "--------";
	}
	cout << endl;
	for (int i = 0; i < processes.size(); i++) {
		int wait = (processes[i].end - processes[i].arrival) - processes[i].length;
		total += wait;
		cout << processes[i].name << "\t" << "waited " << wait << " sec." << endl;
//		printf("%s\twaited %.3f sec.\n", processes[i].name, wait);
	}
	float average = (float) total / (float) processes.size();
	printf("Average waiting time = %.3f sec.\n", average);
}

int main(int argc, char * const argv[]) {
	string filename;
	string alg;
	int slice;
	if (argc != 3 && argc != 4) {
		cout << "Error: Incorrect number of arguments supplied." << endl;
		return -1;
	}
	else {
		filename = argv[1];
		alg = argv[2];
		if (argc == 4)
			slice = stoi(argv[3]); // TODO error checking here to ensure arg is a positive number
	}

	int fd = open(filename.c_str(), O_RDONLY);
	if (fd < 0) {
		cerr << "Error: Could not open file: " << filename << "\n";
		exit(-1);
	}

	int a, b, pnum;
	pnum = 0;

	ifstream infile(filename);
	while (infile >> a >> b) {
		process p;
		p.name = "P" + to_string(pnum);
		p.id = pnum;
		pnum++;
		p.arrival = a;
		p.burst = b;
		p.length = b;
		totaltime += b;
		processes.push_back(p);
	}

	totaltime += processes.at(0).arrival;

	if (alg.compare("RR") == 0) {
		roundrobin(slice);
	}
	else if (alg.compare("SJF") == 0) {
		if (argc == 4) {
			cout << "Error: SJF scheduling does not accept a time slice." << endl;
			return -1;
		}
		shortestjobfirst();
	}
	else {
		cout << "Error: Please enter RR or SJF as a scheduling algorithm." << endl;
		return -1;
	}
}
