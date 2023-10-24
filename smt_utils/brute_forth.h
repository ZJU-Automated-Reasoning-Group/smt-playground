/*
 * brute_forth.h
 *
 *      Author: rainoftime
 */

#pragma once

#include <string>
#include <stack>
#include <map>
#include <vector>
#include <cstdint>
#include <iostream>
#include <vector>
#include <ctime>
#include <thread>
#include <atomic>
#include <set>
#include <math.h>
#include "z3++.h"
#include "z3plus.h"
using namespace z3;
using namespace std;

inline uint64_t snoob(uint64_t sub, uint64_t set) {
	// from hacker's delight
	uint64_t tmp = sub - 1;
	uint64_t rip = set & (tmp + (sub & (0 - sub)) - set);
	for (sub = (tmp & sub) ^ rip; sub &= sub - 1; rip ^= tmp, set ^= tmp)
		tmp = set & (0 - set);
	return rip;
}

bool check_model(expr &exp, context &ctx, vector<func_decl> &decls, uint64_t x,
		unsigned num) {
	//std::cout << "current x is : " << x << std::endl;
	model m(ctx);
	expr bfalse = ctx.bool_val(false);
	expr btrue = ctx.bool_val(true);
	for (auto i = 0; i < num; i++) {
		if (x & 1) {
			m.add_const_interp(decls[i], btrue);
		} else {
			m.add_const_interp(decls[i], bfalse);
		}
		x >>= 1;
	}
	if (m.eval(exp).is_true()) {
		return true;
	} else {
		return false;
	}
}

int solve_with_truth_table(expr &exp, int bound) {
	/*
	 * Solve expr with truth table based brute forth enumeration
	 */
	expr_vector vars(exp.ctx());
	get_expr_vars(exp, vars);
	unsigned var_num = vars.size();
	if (var_num > 15) {
		return 2; // unknown
	} else {
		int ret = 2;
		vector<func_decl> decls;
		for (unsigned i = 0; i < var_num; i++) {
			decls.push_back(vars[i].decl());
		}
		auto set = (1ULL << var_num) - 1;            // n bits
		int check_model_count = 0;
		for (unsigned i = 0; i < var_num + 1; ++i) {
			auto sub = (1ULL << i) - 1;             // i bits
			uint64_t x = sub;
			uint64_t y;
			do {
				// TODO: check_model is not so efficient?
				check_model_count++;
				if (check_model(exp, exp.ctx(), decls, x, var_num)) {
					ret = 1;
					goto RET;
				}
				if (check_model_count > bound)
					goto RET;
				// should be unknown ...
				y = snoob(x, set);                  // get next subset
				x = y;
			} while ((y > sub));
		}
		ret = 0; // all models are tested
		RET: return ret;
	}
}

