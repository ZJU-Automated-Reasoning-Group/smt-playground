/*
 * formula_stats.cpp
 *
 *      Author: rainoftime
 */

#include "formula_stats.h"

#include <algorithm>

void formula_stats::add_function_application(const std::string &name,
		const z3::expr &expr) {
	auto decl_kind = expr.decl().decl_kind();
	if (decl_kind != Z3_OP_BMUL) {
		if (decl_kind != Z3_OP_AND && decl_kind != Z3_OP_OR
				&& decl_kind != Z3_OP_NOT && decl_kind != Z3_OP_IMPLIES) {
			function_symbols.insert(name);
		}
	} else {
		if (expr.num_args() == 2
				&& (expr.arg(0).is_numeral() || expr.arg(0).is_numeral())) {
			function_symbols.insert("bvmul(scalar)");
		} else {
			function_symbols.insert("bvmul");
		}
	}

	if (expr.is_bv()) {
		max_bitwidth = std::max(max_bitwidth, expr.get_sort().bv_size());
	}
}

void formula_stats::add_constant(const std::string &name, const z3::sort &s) {
	constants.insert(name);

	if (s.is_bool()) {
		max_bitwidth = std::max(max_bitwidth, 1u);
	} else if (s.is_bv()) {
		max_bitwidth = std::max(max_bitwidth, s.bv_size());
	}
}

void formula_stats::add_variable(const std::string &name, const z3::sort &s) {
	variables.insert(name);

	if (s.is_bool()) {
		max_bitwidth = std::max(max_bitwidth, 1u);
	} else if (s.is_bv()) {
		max_bitwidth = std::max(max_bitwidth, s.bv_size());
	}
}

void formula_stats::add_numeral(const std::string &text, const z3::expr &expr) {
	numerals.insert(text);
}

std::ostream& operator<<(std::ostream &os, const formula_stats &stats) {
	std::cout << stats.constants.size() << ",";
	std::cout << stats.variables.size() << ",";
	std::cout << stats.numerals.size() << ",";
	std::cout << stats.max_bitwidth << ",";

	unsigned int i = 0;
	for (const auto &fun : stats.function_symbols) {
		os << fun;
		i++;
		if (i != stats.function_symbols.size()) {
			os << " ";
		}
	}

	return os;
}

