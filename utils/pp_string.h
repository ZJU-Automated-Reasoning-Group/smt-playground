#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <random>
#include <signal.h>
#include <sys/stat.h>

// For generating random CNF name when used in concurrent envirooment
//   e.g., different SAT engines run in parallel
std::string const default_chars =
		"abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

std::string random_string(size_t len = 15, std::string const &allowed_chars =
		default_chars) {
	std::mt19937_64 gen { std::random_device()() };

	std::uniform_int_distribution<size_t> dist { 0, allowed_chars.length() - 1 };

	std::string ret;

	std::generate_n(std::back_inserter(ret), len, [&] {
		return allowed_chars[dist(gen)];
	});
	return ret;
}
