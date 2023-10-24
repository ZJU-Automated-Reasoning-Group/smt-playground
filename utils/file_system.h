#pragma once

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iterator>

#include <glob.h>
#include <dirent.h>

using namespace std;

// E.g.  pattern is /home/rot/*.smt2
void get_files(const string &pattern, vector<string> &files) {
	glob_t glob_result;
	glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
	for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
		files.push_back(string(glob_result.gl_pathv[i]));
	}
	globfree(&glob_result);
}

bool file_exists(char *filename) {
	struct stat buf;
	if (stat(filename, &buf) != -1) {
		return true;
	}
	return false;
}

template<typename Out>
void split_string(std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split_string(std::string &s, char delim) {
	std::vector<std::string> elems;
	split_string(s, delim, std::back_inserter(elems));
	return elems;
}

