#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

using namespace std;

int num_threads = 1; // default num of threads to 1
long sum = 0;
long numbers[1000000] = { 0 }; // where we will store the list of numbers
int size = 0; // number of ints in the file

void do_sum(int i, vector<long>* sums, int number, int b, int split) {
	int elements;
	long first;
	long last; 

	if (b == 0) { // first half of the groups
		elements = (number / num_threads) + 1;
		first = i * elements;
		last = first + elements - 1;
	}
	else { // remaining half of the groups
		elements = number / num_threads;
		first = split * (elements + 1) + ((i - split) * (elements));
		last = first + elements - 1;
	}

	long temp = 0;

	for (long i = first; i <= last; i++) {
		temp += numbers[i];
	}
	
	sums->at(i) += temp;
}

int main(int argc, char const *argv[]) {

	ifstream file(argv[1]);

	int num;

	if (file.good()) {
		while (file >> num) {
			numbers[size] = num;
			size++;
		}
	}
		

	num_threads = stoi(argv[2]); // get number of threads argument
	vector<long int> sums; // where we will store the sums for each thread

	for (int i = 0; i < num_threads; i++) {
		sums.push_back(0); // start out with sums of 0
	}

	thread t[num_threads];
	int split = size % num_threads;

	for (int i = 0; i < num_threads; i++) {
		if (i < split) {
			t[i] = thread(do_sum, i, &sums, size, 0, split);
		}
		else {
			t[i] = thread(do_sum, i, &sums, size, 1, split);
		}
	}

	for (int i = 0; i < num_threads; i++) {
		t[i].join();
	}

	for (int i = 0; i < num_threads; i++) {
		cout << "Thread " << i + 1 << ": " << sums[i] << endl;
		sum += sums[i];
	}

	cout << "Sum = " << sum << endl;

	return 0;
}
