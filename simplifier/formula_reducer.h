#pragma once


#include "z3++.h"


class formula_reducer
{
public:
    z3::expr reduce(const z3::expr& e, unsigned int newBW);
};

