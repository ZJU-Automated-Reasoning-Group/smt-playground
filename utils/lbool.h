#pragma once

#include <iostream>

typedef enum {
	l_false = -1, l_undef, l_true
} lbool;

inline lbool operator~(lbool lb) {
	return static_cast<lbool>(-static_cast<int>(lb));
}

inline lbool to_pp_lbool(bool b) {
	return static_cast<lbool>(static_cast<int>(b) * 2 - 1);
}

std::ostream& operator<<(std::ostream &out, lbool b);

/**
 \brief Convert l_true -> satisfiable, l_false -> unsatisfiable, and l_undef -> unknown.
 */
char const* to_sat_str(lbool l);
