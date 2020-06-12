/**********************************************
 * Last Name:   Roebuck
 * First Name:  Jasmine
 * Student ID:  30037334
 * Course:      Operating Systems
 * Tutorial:    T03
 * Assignment:  1
 * Question:    Q5
 *
 * File name:   myWc.cpp
 *********************************************/

#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main (int argc, char * const argv[])
{
  // get the file name from command line
  string filename;
  if (argc != 2) {
    cerr << "Usage: readFile <input file> " << endl;
    return -1;
  }
  else {
    filename = argv[1];
  }
  
  // open the file for reading
  int fd = open( filename.c_str(), O_RDONLY);
  if( fd < 0) {
    cerr << "Could not open file " << filename << "\n";
    exit(-1);
  }

  // read entire file into a buffer
  int count = 0;
  char buffer[1000000];
  read(fd, &buffer, 1000000);

  // count newline characters in the buffer
  for(int i = 0; i < 1000000; i++) {
      if (buffer[i] == '\0') {
		 break;
      }
      if (buffer[i] == '\n') {
		 count++;
      }
  }

  // close file and report results
  close( fd);
  cout << count << " " << filename << "\n";
  return 0;
}
