#include "lbool.h"

std::ostream& operator<<(std::ostream &out, lbool b) {
	switch (b) {
	case l_false:
		return out << "l_false";
	case l_true:
		return out << "l_true";
	default:
		return out << "l_undef";
	}
}

char const* to_sat_str(lbool l) {
	switch (l) {
	case l_false:
		return "unsat";
	case l_true:
		return "sat";
	default:
		return "unknown";
	}
}
