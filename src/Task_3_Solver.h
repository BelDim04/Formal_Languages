//
// Created by dmitriy on 14.10.22.
//

#ifndef FORMAL_LANGUGES_TASK_3_SOLVER_H
#define FORMAL_LANGUGES_TASK_3_SOLVER_H
#include "Includes.h"
#include "NFA_from_regex_builder.h"
#include "NFA.h"

class Task_3_Solver {
    static bool dfs(int v, FDFA& pdka, std::vector<bool>& used);

public:
    static bool task3(const std::string& reg, char c, int k);
};


#endif //FORMAL_LANGUGES_TASK_3_SOLVER_H
