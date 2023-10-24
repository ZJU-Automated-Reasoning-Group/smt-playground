#include "opt/opt.h"
#include "opt/opt_util.h"
#include <map>
#include <cstdint>

using namespace std;

void opt_solver::test_sym_abs() {
    // for single objectives
    context ctx;

    expr x = ctx.bv_const("x", 8);
    expr y = ctx.bv_const("y", 8);
    expr z = ctx.bv_const("z", 8);

    unsigned z3opt_best = 0;
    double z3opt_total_time = 0;

    unsigned linear_search_best = 0;
    double linear_search_total_time = 0;

    unsigned binary_search_best = 0;
    double binary_search_total_time = 0;

    //z3::set_param("inc_qfbv", 4);

# if 0
    m_verbose_lvl = 1;
    for (unsigned i = 6; i < 50; i++) {
        expr pre_cond(ctx);
        //expr val_two = ctx.bv_val(2, 16);
        //expr add_nof = bvadd_no_overflow(x, val_two); // TODO: how come the ret is a predicate?..
        //std::cout << add_nof << std::endl;
        try {
            pre_cond =  uge(x, 3) && ule(x + 2, i);
        } catch (z3::exception& ex) {
            std::cout << ex.msg() << std::endl;
            exit(0);
        }

        expr query = x;
        expr_vector res(ctx);
        //solve_with_qsmt(pre_cond, query, res, g_max);
        solve_with_z3opt(pre_cond, query, res, g_max);
        solve_with_linear_search(pre_cond, query, res, g_max);
        solve_with_binary_search(pre_cond, query, res, g_max);
        if (m_linear_search_max != m_binary_search_max) {
            std::cout << " Inconsistent!!!\n";
            std::cout << pre_cond << std::endl;
            std::cout << i << std::endl;
        }
        z3opt_total_time += m_z3opt_time;
        linear_search_total_time += m_linear_search_time;
        binary_search_total_time += m_binary_search_time;

        if (m_z3opt_time <= m_linear_search_time && m_z3opt_time <= m_binary_search_time) {
            z3opt_best++;
        } else if (m_linear_search_time <= m_z3opt_time && m_linear_search_time <= m_binary_search_time) {
            linear_search_best++;
        } else if (m_binary_search_time <= m_z3opt_time && m_binary_search_time <= m_linear_search_time) {
            binary_search_best++;
        }

        // reset stat
        m_z3opt_time = 0;
        m_z3opt_max = 0;

        m_linear_search_time = 0;
        m_linear_search_solver_call = 0;
        m_linear_search_max = 0;

        m_binary_search_time = 0;
        m_binary_search_solver_call = 0;
        m_binary_search_max = 0;
        //solve_with_partitioned_search(pre_cond, query, res, g_max);
    }
    std::cout << " z3 opt time: " <<  z3opt_total_time <<std::endl;
    std::cout << " lin time: " <<   linear_search_total_time <<std::endl;
    std::cout << " bin time: " <<  binary_search_total_time <<std::endl;
    std::cout << " z3 opt best: " <<  z3opt_best <<std::endl;
    std::cout << " lin best: " <<   linear_search_best <<std::endl;
    std::cout << " bin best: " <<  binary_search_best <<std::endl;
#else
    m_verbose_lvl = 1;

    expr pre_cond = ule(x, 216) && ule(y, 33) && ule(z, 111);

    expr_vector queries(ctx);
    queries.push_back(x);
    queries.push_back(y);
    queries.push_back(z);

    std::vector<expr_vector> multi_res;

    expr_vector res_x(ctx); multi_res.push_back(res_x);
    expr_vector res_y(ctx); multi_res.push_back(res_y);
    expr_vector res_z(ctx); multi_res.push_back(res_z);


    expr_vector tmp_res(ctx);
    //solve_with_z3opt(pre_cond, x, tmp_res, g_max);
    //solve_with_z3opt(pre_cond, y, tmp_res, g_max);
    //solve_with_z3opt(pre_cond, z, tmp_res, g_max);

    //solve_with_binary_search(pre_cond, x, tmp_res, g_max);
    //solve_with_binary_search(pre_cond, y, tmp_res, g_max);
    //solve_with_binary_search(pre_cond, z, tmp_res, g_max);

    solve_with_compact_search(pre_cond, queries, multi_res, g_max);

#endif

}

void opt_solver::test_sym_abs_multi_obj() {
    //z3::set_param("inc_qfbv", 4);
    context ctx;
    std::cout << ".........\n";
    expr u = ctx.bv_const("u", 16);
    expr w = ctx.bv_const("w", 16);
    expr x = ctx.bv_const("x", 16);
    expr y = ctx.bv_const("y", 16);
    expr z = ctx.bv_const("z", 16);

    double z3opt_total_time = 0;
    double linear_search_total_time = 0;
    double binary_search_total_time = 0;
    double compact_search_total_time = 0;

    m_verbose_lvl = 0;
    expr pre_cond(ctx);
    for (unsigned i = 6; i < 10000; i = i + 100) {
        expr pre_cond =  uge(x, 2) && ule(x, i) && ule(y, i + 400) && ule(z, i + 30000)
        		&& w == 135 && uge(u, 20);

        expr_vector tmp_res(ctx);
        solve_with_z3opt(pre_cond, x, tmp_res, g_max); z3opt_total_time += m_z3opt_time;
        solve_with_z3opt(pre_cond, y, tmp_res, g_max); z3opt_total_time += m_z3opt_time;
        solve_with_z3opt(pre_cond, z, tmp_res, g_max); z3opt_total_time += m_z3opt_time;
        solve_with_z3opt(pre_cond, w, tmp_res, g_max); z3opt_total_time += m_z3opt_time;
        solve_with_z3opt(pre_cond, u, tmp_res, g_max); z3opt_total_time += m_z3opt_time;

        solve_with_linear_search(pre_cond, x, tmp_res, g_max); linear_search_total_time += m_linear_search_time;
        solve_with_linear_search(pre_cond, y, tmp_res, g_max); linear_search_total_time += m_linear_search_time;
        solve_with_linear_search(pre_cond, z, tmp_res, g_max); linear_search_total_time += m_linear_search_time;
        solve_with_linear_search(pre_cond, w, tmp_res, g_max); linear_search_total_time += m_linear_search_time;
        solve_with_linear_search(pre_cond, u, tmp_res, g_max); linear_search_total_time += m_linear_search_time;

        solve_with_binary_search(pre_cond, x, tmp_res, g_max); binary_search_total_time += m_binary_search_time;
        solve_with_binary_search(pre_cond, y, tmp_res, g_max); binary_search_total_time += m_binary_search_time;
        solve_with_binary_search(pre_cond, z, tmp_res, g_max); binary_search_total_time += m_binary_search_time;
        solve_with_binary_search(pre_cond, w, tmp_res, g_max); binary_search_total_time += m_binary_search_time;
        solve_with_binary_search(pre_cond, u, tmp_res, g_max); binary_search_total_time += m_binary_search_time;

        expr_vector queries(ctx);
        queries.push_back(x); queries.push_back(y); queries.push_back(z);
        queries.push_back(w); queries.push_back(u);
        std::vector<expr_vector> multi_res;
        expr_vector res_x(ctx); multi_res.push_back(res_x);
        expr_vector res_y(ctx); multi_res.push_back(res_y);
        expr_vector res_z(ctx); multi_res.push_back(res_z);
        expr_vector res_w(ctx); multi_res.push_back(res_w);
        expr_vector res_u(ctx); multi_res.push_back(res_u);

        solve_with_compact_search(pre_cond, queries, multi_res, g_max); compact_search_total_time += m_compact_search_time;

        // reset stat
        m_z3opt_time = 0;
        m_linear_search_time = 0;
        m_binary_search_time = 0;
        m_compact_search_time = 0;
    }
    std::cout << " + z3opt time: " <<  z3opt_total_time <<std::endl;
    std::cout << " + lin time: " <<   linear_search_total_time <<std::endl;
    std::cout << "    solver calls : " << m_linear_search_solver_call << std::endl;
    std::cout << " + bsearch time: " <<   binary_search_total_time <<std::endl;
    std::cout << "    solver calls : " << m_binary_search_solver_call << std::endl;
    std::cout << " + compact time: " <<  compact_search_total_time <<std::endl;
    std::cout << "    solver calls : " << m_compact_check_solver_call << std::endl;
}

void opt_solver::solve_file(char const* filename) {
    context ctx;
    optimize opt(ctx);
    opt.from_file(filename);

    std::cout << opt <<  std::endl;

    // the constraint
    expr pre_cond = mk_and(opt.assertions());
    // objs
    expr_vector objs = opt.objectives();
    for (unsigned i = 0; i < objs.size(); i++) {
        expr obj_i = objs[i];
        std::cout << obj_i << std::endl;
        expr_vector res_i(ctx);

        // convert max(-x) to min(x)
        if (obj_i.decl().name().str() == "bvneg") {
            obj_i = obj_i.arg(0);
            solve_with_z3opt(pre_cond, obj_i, res_i, g_min);
        } else {
            solve_with_z3opt(pre_cond, obj_i, res_i, g_max);
        }
    }
}
