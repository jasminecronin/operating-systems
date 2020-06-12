#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

int main( int argc, char ** argv) {
	  // handle command line arguments
	  if( argc != 3) {
	  	fprintf(stderr, "Incorrect number of arguments.\n");
	        exit(-1);
	  }
	  string suffix = argv[1];
	  int num = stoi(argv[2]);
	  
	  // open 'find...'
	  FILE * fp = popen( "./myFind", "r");
	  if( fp == NULL) {
	        perror("popen failed:");
	        exit(-1);
	  }
	  
	  // read in all filenames
	  char buff[MAX_FNAME_SIZE];
	  int nFiles = 0;
	  char * files[MAX_FILES];
	  while(fgets(buff,MAX_FNAME_SIZE,fp)) {
	        int len = strlen(buff) - 1;
	        files[nFiles] = strndup(buff,len);
	        nFiles ++;
	  }
	  fclose(fp);
	  
	  // get file sizes for each file and test if they have the required suffix
	  // if so, put them onto a vector of pairs
	  long long totalSize = 0;
	  struct stat st;
	  vector<pair<long, string>> sorted;

	  for(int i = 0 ; i < nFiles ; i ++ ) {
	        if( 0 != stat(files[i],&st)) {
	        	perror("stat failed:");
	        	exit(-1);
	  	}
		string fn(files[i]); // convert char array to string
		if (fn.substr(fn.length() - suffix.length()) == suffix) { // if filename matches suffix
			sorted.push_back(make_pair(st.st_size, fn)); // add to list to be sorted
			totalSize += st.st_size;
		}
	  }
	  sort(sorted.begin(), sorted.end()); // sort the vector by first element
	  for (int i = 0; i < num; i++) { // print
		if (i >= sorted.size()) {break;}
		cout << sorted.at(sorted.size() - 1 - i).second << " " << sorted.at(sorted.size() - 1 - i).first << endl;
	  }

	  printf("Total size: %lld \n", totalSize);
	  // clean up
	  for(int i = 0; i < nFiles ; i ++ ) {
	        free(files[i]);
	  }
	  // return success
	  return 0;
}
