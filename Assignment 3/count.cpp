/*******************************************
 * Last Name: Roebuck
 * First Name: Jasmine
 * Course: Operating Systems
 * Tutorial: 3
 * Assignment: 3
 * Question: Q5
 *
 * File Name: count.cpp
 * *****************************************/

/// counts number of primes from standard input
///
/// compile with:
///	$ gcc findPrimes.c -O2 -o count -lm
///

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <pthread.h>
#include <thread>
#include <iostream>
#include <algorithm>

using namespace std;

vector<int64_t> smallNums;
vector<int64_t> largeNums;
int nThreads = 1;
int64_t counting = 0;
volatile bool prime = true;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/// primality test, if n is prime, return 1, else return 0
int isPrime(int64_t n, int64_t first, int64_t last) {
	//cout << "Max " << n << endl;
	if (n <= 1) return 0; // small numbers are not primes
	if (n <= 3) return 1; // 2 and 3 are prime
	if (n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and 3 are not primes
	int64_t i = first;
	//cout << i << " " << first << endl;
	i = i - ((i - 5) % 6);
	if (i < 5) i = 5;
	int64_t max = last;
	// TODO need to check the first number in the range
	// make sure it falls on one of the i + 6 number to start the range correctly
	//if (max > sqrt(n)) max = sqrt(n);
	//cout << "First: " <<  i << " Last: " << max << endl;
	//cout << max << " " << last << endl;
	//i = i - ((i - 5) % 6);
	while (i <= max) {
		//cout << i << endl;
		if (prime == false) return 0; // thread signal
		//if (n % i == 0) return 0;
		//i++;
		if (n % i == 0 || n % (i + 2) == 0) return 0;
		i += 6;
	}
	return 1;
}

void smallPrimes(int i, int b, int split) {
	int elements;
	long first;
	long last;

	if (b == 0) {
		elements = (smallNums.size() / nThreads) + 1;
		first = i * elements;
		last = first + elements - 1;
	} else {
		elements = smallNums.size() / nThreads;
		first = split * (elements + 1) + ((i - split) * elements);
		last = first + elements - 1;
	}

	//cout << first << " " << last << " " << endl;
	//cout << "Thread " << i << " doing " << first << " to " << last << endl;

	for (long i = first; i <= last; i++) {
		//cout << smallNums[i] << endl;
		//cout << isPrime(smallNums[i]) << endl;
		int64_t f = 1;
		int64_t l = smallNums[i];
		l = sqrt(l);
		if (isPrime(smallNums[i], f, l) == 1) {
			pthread_mutex_lock(&mutex);
			counting++;
			pthread_mutex_unlock(&mutex);
			//cout << smallNums[i] << endl;
		}
	}
}

void largePrimes(int i, int64_t number) {
	int64_t max = sqrt(number) + 1;
	int64_t first = ((max / nThreads) * i) + 1;
	int64_t last = (max / nThreads) * (i + 1);

	//cout << "Thread " << i << " doing " << first << " to " << last << endl;

	if (isPrime(number, first, last) == 0) {
		pthread_mutex_lock(&mutex);
		prime = false;
		pthread_mutex_unlock(&mutex);
	}
	
	//if (isPrime(number, first, last)) {
	//	pthread_mutex_lock(&mutex);
	//	counting++;
	//	pthread_mutex_unlock(&mutex);
	//}
}

int main(int argc, char ** argv) {
	// parse the file. add small numbers to one vector, big numbers to another
	// have all threads split the list of small numbers (each take one)
	// then have all threads work together on the large numbers
	// 	will need signaling when one thread finds a divisor

	/// parse command line arguments
	if (argc != 1 && argc != 2) {
		printf("Usage: countPrimes [nThreads]\n");
		exit(-1);
	}
	if (argc == 2) nThreads = atoi(argv[1]);
	//cout << nThreads << endl;

	/// handle invalid arguments
	if (nThreads < 1 || nThreads > 256) {
		printf("Bad arguments. 1 <= nThreads <= 256!\n");
	}
	//if (nThreads != 1) {
	//	printf("I am not multithreaded yet :-(\n");
	//	exit(-1);
	//}

	/// count the primes
	printf("Counting primes using %d thread%s.\n", nThreads, nThreads == 1 ? "" : "s");
	while(1) {
		int64_t num;
		if (1 != scanf("%ld", & num)) break;
		if (num > 1000000) {
			largeNums.push_back(num);
		} else {
			smallNums.push_back(num);
		}
		//if (isPrime(num)) count++;
	}

	//pthread_t t[nThreads];
	thread t[nThreads];
	int split = smallNums.size() % nThreads;
	for (int i = 0; i < nThreads; i++) {
		if (i < split) {
			t[i] = thread(smallPrimes, i, 0, split);
			//pthread_create(&t[i], NULL, smallPrimes, i, 0, split);
		} else {
			t[i] = thread(smallPrimes, i, 1, split);
			//pthread_create(&t[i], NULL, smallPrimes, i, 1, split);
		}
	}

	for (int i = 0; i < nThreads; i++) {
		t[i].join();
		//pthread_join(t[i], NULL);
	}

	// create a new set of threads to deal with very large numbers

	for (int j = 0; j < largeNums.size(); j++) {
		for (int i = 0; i < nThreads; i++) {
			//pthread_create(&t[i], NULL, largePrimes, i, largeNums[j]);
			t[i] = thread(largePrimes, i, largeNums[j]);
		}

		for (int i = 0; i < nThreads; i++) {
			//pthread_join(t[i], NULL);
			t[i].join();
		}
		if (prime) { 
			counting++;
			//cout << largeNums[j] << endl;
		}
		prime = true;
	}

	/// report results
	printf("Found %ld primes.\n", counting);

	return 0;
}
