#pragma once

#include "z3++.h"
#include "formula_stats.h"
#include <set>

template<class T>
class formula_traverser {
public:
	formula_traverser() {
	}

	void traverse(const z3::expr&);

	T get_data() {
		return data;
	}

private:
	T data;

	std::set<Z3_ast> processed_cache;
};

template<class T>
void formula_traverser<T>::traverse(const z3::expr &expr) {
	auto item = processed_cache.find((Z3_ast) expr);
	if (item != processed_cache.end()) {
		return;
	}

	if (expr.is_numeral()) {
		std::stringstream ss;
		ss << expr;

		data.add_numeral(ss.str(), expr);
	} else if (expr.is_const()) {
		std::stringstream ss;
		ss << expr;

		if (ss.str() == "true" || ss.str() == "false") {
			return;
		}

		data.add_constant(ss.str(), expr.get_sort());
	} else if (expr.is_app()) {
		z3::func_decl f = expr.decl();
		unsigned num = expr.num_args();

		if (num != 0) {
			data.add_function_application(f.name().str(), expr);
		}

		for (unsigned i = 0; i < num; i++) {
			traverse(expr.arg(i));
		}
	} else if (expr.is_quantifier()) {
		Z3_ast ast = (Z3_ast) expr;

		int bound_variables = Z3_get_quantifier_num_bound(expr.ctx(), ast);

		for (int i = 0; i < bound_variables; i++) {
			Z3_symbol z3_symbol = Z3_get_quantifier_bound_name(expr.ctx(), ast,
					i);
			Z3_sort z3_sort = Z3_get_quantifier_bound_sort(expr.ctx(), ast, i);

			z3::symbol current_symbol(expr.ctx(), z3_symbol);
			z3::sort current_sort(expr.ctx(), z3_sort);

			data.add_variable(current_symbol.str(), current_sort);
		}

		traverse(expr.body());
	}

	processed_cache.insert((Z3_ast) expr);
}

