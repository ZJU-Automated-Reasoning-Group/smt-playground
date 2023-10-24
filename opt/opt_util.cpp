#include "opt_util.h"
#include <map>
#include <cstdint>

void optutil::get_k_models(z3::expr &exp, int k) {
	z3::context &ctx = exp.ctx();
	z3::solver solver(ctx);
	unsigned timeout = m_timeout;
	params p(ctx);
	p.set("timeout", timeout);
	solver.set(p);
	solver.add(exp);
	while (solver.check() == z3::sat && k >= 1) {
		std::cout << solver << std::endl;
		z3::model m = solver.get_model();
		z3::expr_vector args(ctx);
		for (unsigned i = 0; i < m.size(); i++) {
			// get z3 variable
			z3::func_decl z3Variable = m[i];
			std::string varName = z3Variable.name().str();
			z3::expr exp = m.get_const_interp(z3Variable);
			unsigned bvSize = exp.get_sort().bv_size();
			int value = m.eval(exp).get_numeral_int();
			// std::string svalue = Z3_get_numeral_string(ctx, exp);
			if (exp.get_sort().is_bv()) {
				args.push_back(
						ctx.bv_const(varName.c_str(), bvSize)
								!= ctx.bv_val(value, bvSize));
			}
		}
		// block current model
		solver.add(mk_or(args));
		k--;
	}
}

bool optutil::is_var_fixed(expr &exp, expr &var) {
	z3::context &ctx = exp.ctx();
	if (m_var_lge_zero) {
		exp = exp && var >= 0;
	}

	z3::solver solver(ctx);
	unsigned timeout = m_timeout;
	params p(ctx);
	p.set("timeout", timeout);
	solver.set(p);

	solver.add(exp);

	if (solver.check() == z3::sat) {
		// TODO:
		model m = solver.get_model();
		std::string varName = var.decl().name().str();
		expr var_val = m.eval(var);
		solver.add(
				ctx.bv_const(varName.c_str(), var.get_sort().bv_size())
						!= var_val);
		if (solver.check() == z3::unsat) {
			return true;
		} else {
			return true;
		}
	} else {
		return false;
	}
}
