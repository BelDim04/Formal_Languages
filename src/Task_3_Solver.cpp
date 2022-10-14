//
// Created by dmitriy on 14.10.22.
//

#include "Task_3_Solver.h"

bool Task_3_Solver::dfs(int v, FDFA &pdka, std::vector<bool> &used) {
    used[v] = true;
    bool res = false;
    if(pdka.is_final[v]) return true;
    for(int u: pdka.vertices[v].edges){
        if(!used[u]) res |= dfs(u, pdka, used);
    }
    return res;
}

bool Task_3_Solver::task3(const std::string& reg, char c, int k) {
    if(c < 'a' || c >= 'a' + AlphabetSize){
        throw std::runtime_error("invalid symbol");
    }
    auto pdka = NFA(NFA_from_regex_builder::build_NFA(reg)).to_PDKA();
    pdka.minimize();
    int cur_v = pdka.start_vertex;
    for(int i = 0; i < k; ++i){
        cur_v = pdka.vertices[cur_v].edges[c - 'a'];
    }
    std::vector<bool> used(pdka.vertices.size(), false);
    return dfs(cur_v, pdka, used);
}
