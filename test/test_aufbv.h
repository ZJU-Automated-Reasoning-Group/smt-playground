#pragma once

#include "test_util.h"


void test_aufbv() {
    context ctx;
    set_param("verbose", "15");
    solver s1 = tactic(ctx, "pp_qfaufbv_light").mk_solver();  // qfauf
    solver s2 = tactic(ctx, "qfaufbv").mk_solver();
    params p(ctx);
    p.set("timeout", (unsigned)5000);
    s1.set(p);
    s2.set(p);
#if 0
    Z3_ast aa = NULL;
    try {
        aa = Z3_parse_smtlib2_file(ctx, m_smt_file_path.c_str(), 0, 0, 0, 0, 0, 0);
    } catch (z3::exception& ex) {
        std::cout << ex.msg() << std::endl;
        throw ex;
    }
    expr e(ctx, aa);
#else
    expr_vector evec = ctx.parse_file("/home/rainoftime/Work/SMT/Bench/QF_AUFBV/ecc/com.galois.ecc.P384ECC64.sq_inner22.short.smt2");
    expr e = mk_and(evec);
#endif
    s1.add(e);
    s2.add(e);
    s1.check();
    //s2.check();
}

