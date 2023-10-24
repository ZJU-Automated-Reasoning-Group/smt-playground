#pragma once

#include "test_util.h"

bool test_equivalence(expr e1, expr e2) {
    solver s1(e1.ctx());
    solver s2(e1.ctx());
    s1.add(not(implies(e1, e2)));
    s2.add(not(implies(e2, e1)));
    if (s1.check() == sat) {
        std::cout << "models: \n";
        std::cout << s1.get_model() << std::endl;
    }
    if (s1.check() == unsat && s2.check() == unsat) {
        return true;
    }
    return false;
}


// TODO: seems elim-small-bv does not produce an equivalent form?
void test_qe() {
    context c;
    //set_param("verbose", 100000);
    tactic qe = tactic(c, "qe");
    tactic qe_expa = tactic(c, "elim-small-bv");
    tactic qe_lite = tactic(c, "qe-light");
    tactic qe2 = tactic(c, "qe2");
    tactic qe_new = tactic(c, "qe_new");
    expr a = c.bv_const("a", 4);
    expr b = c.bv_const("b", 4);
    expr x = c.bv_const("x", 4);
    expr y = c.bv_const("y", 4);
    expr f = implies(x <= a, x < b);
    expr ff = x + y < 3;


    expr qf = exists(x, ff);

    goal g(c);
    solver s1(c);
    solver s2(c);
    g.add(qf);
    std::cout << g << std::endl;

    CaluTime timer;
    timer.TimerBegin();
    expr qf1 =  qe2(g)[0].as_expr();
    std::cout << qf1 << std::endl;
    timer.TimerEnd();
    std::cout << "time: " << timer.TimerSpan() << std::endl;

    timer.TimerBegin();
    expr qf2 =  qe_new(g)[0].as_expr();
    std::cout << qf2 << std::endl;
    timer.TimerEnd();
    std::cout << "time: " << timer.TimerSpan() << std::endl;

    std::cout << "equiv: " << test_equivalence(qf, qf1) << std::endl;
    std::cout << "equiv: " << test_equivalence(qf, qf2) << std::endl;

}

void test_qe2() {
    context c;
    set_param("verbose", 10);
    tactic qe = tactic(c, "qe");
    tactic qe_expa = tactic(c, "elim-small-bv");
    tactic qe2 = tactic(c, "qe2");
    //tactic qe_new = tactic(c, "qe_new");
    //*
    expr x = c.bv_const("x", 4);
    expr y = c.bv_const("y", 4);
     //*/
    // why different results when applied to int, bv, and real.
    /*
    expr x = c.int_const("x");
    expr y = c.int_const("y");
    */
    expr f1 = x + y > 5 && y < 2;
    expr qf1 = exists(y, f1);
    solver s(c);
    s.add(qf1);
    std::cout << s.check() << std::endl;
    std::cout << s.get_model() << std::endl;

    goal g(c);
    g.add(qf1);
//std::cout << g << std::endl;

    CaluTime timer;
    timer.TimerBegin();
    expr qff1 =  qe2(g)[0].as_expr();
    std::cout << qff1 << std::endl;
    timer.TimerEnd();
    std::cout << "time: " << timer.TimerSpan() << std::endl;

}


void test_qe3() {
    context c;
    set_param("verbose", 100);
    tactic qe = tactic(c, "qe");
    tactic qe2 = tactic(c, "qe2");

    expr x = c.bv_const("x", 32);
    expr y = c.bv_const("y", 32);
    //*/
    // why different results when applied to int, bv, and real.
    /*
    expr x = c.int_const("x");
    expr y = c.int_const("y");
    */
    expr f1 = x + x*y > 5 && y < 1;
    expr qf1 = exists(y, f1);

    solver s = tactic(c, "qsat").mk_solver();
    s.add(qf1);
    std::cout << s.check() << std::endl;

    goal g(c);
    g.add(qf1);
//std::cout << g << std::endl;
/*
    CaluTime timer;
    timer.TimerBegin();
    expr qff1 =  qe2(g)[0].as_expr();
    std::cout << qff1 << std::endl;
    timer.TimerEnd();
    std::cout << "time: " << timer.TimerSpan() << std::endl;

*/

}
