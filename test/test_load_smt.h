#pragma once

#include "test_util.h"


//  find all the predicates (func_decls with Boolean range)

/*
 *
Z3_ast ast = Z3_parse_smtlib2_file(c, filename.c_str(), 0, 0, 0, 0, 0, 0);
expr e(c, ast);
vector<func_decl> my_fds;
collect_fds(e, my_fds);
 */
void collect_fds(const expr& e, vector<func_decl>& fds) {
    if (e.is_app()) {
        func_decl fd = e.decl();
        if (fd.range().is_bool()) {
            fds.push_back(fd);
        }
        for (unsigned i = 0; i < e.num_args(); i++) {
            collect_fds(e.arg(i), fds);
        }
    }
    else if (e.is_quantifier()) {
        collect_fds(e.body(), fds);
    }
}

// find all variables
void collect_vars() {

}
