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

    static bool is_alphabet_symbol(char c);

public:
    static bool task3_with_automat(const std::string& reg, char c, int k);

    static bool task3_with_stack(const std::string& reg, char c, int k);
};


#endif //FORMAL_LANGUGES_TASK_3_SOLVER_H
