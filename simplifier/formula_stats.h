#pragma once

#include "z3++.h"
#include <set>
#include <iostream>

class formula_stats {
public:
	formula_stats() {
	}

	void add_function_application(const std::string&, const z3::expr&);
	void add_constant(const std::string&, const z3::sort&);
	void add_variable(const std::string&, const z3::sort&);
	void add_numeral(const std::string&, const z3::expr&);

	friend std::ostream& operator<<(std::ostream &os,
			const formula_stats &stats);

	std::set<std::string> function_symbols;
	unsigned int max_bitwidth = 0;

	std::set<std::string> constants;
	std::set<std::string> variables;
	std::set<std::string> numerals;

	unsigned int numeral_count = 0;
};
