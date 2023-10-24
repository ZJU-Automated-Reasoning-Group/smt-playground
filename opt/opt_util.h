#pragma once


#include <unordered_map>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cstdint>
#include <string>
#include "z3++.h"
#include "z3.h"

using namespace z3;
using namespace std;



class optutil {
    unsigned    m_timeout = 3000;       // three seconds

    bool        m_var_lge_zero = true;  // we will assume every queried var is >= 0

private:
    void get_k_models(expr& exp, int k);

    bool is_var_fixed(expr& pre_cond, expr& var);

    void partition_iterval(int64_t low, int64_t upp, int64_t k);

};
