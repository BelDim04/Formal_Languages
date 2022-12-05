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

bool Task_3_Solver::task3_with_automat(const std::string& reg, char c, int k) {
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

bool Task_3_Solver::task3_with_stack(const std::string &reg, char x, int k) {
    if(x < 'a' || x >= 'a' + AlphabetSize){
        throw std::runtime_error("invalid symbol");
    }
    std::stack< std::pair<int, int> > temp_prefix;
    for (auto c: reg) {
        if (is_alphabet_symbol(c)) {
            if(c == x) {
                temp_prefix.push({1, 1});
            }
            else{
                temp_prefix.push({0, 0});
            }
        } else if (c == '.' || c == '+') {
            if(temp_prefix.size() < 2) {
                throw std::runtime_error("invalid regex");
            }
            auto a = temp_prefix.top();
            temp_prefix.pop();
            if (c == '+') {
                temp_prefix.top().first = std::max(temp_prefix.top().first, a.first);
                if((temp_prefix.top().second == -1 && a.second == 0) || (temp_prefix.top().second == -1 && a.second == 0)) temp_prefix.top().second = -1;
                else temp_prefix.top().second = std::max(temp_prefix.top().second, a.second);
            }
            else{
                int pref = std::max(temp_prefix.top().first, temp_prefix.top().second? temp_prefix.top().second + a.first: 0);
                int full = temp_prefix.top().second && a.second ? std::max(temp_prefix.top().second, 0) + std::max(a.second, 0) : 0;
                temp_prefix.top() = {pref, full};
            }
        } else if (c == '*') {
            if(temp_prefix.empty()) {
                throw std::runtime_error("invalid regex");
            }
            if(temp_prefix.top().second){
                temp_prefix.top().first = k;
                temp_prefix.top().second = k;
            }
            else{
                temp_prefix.top().second = -1;
            }
        } else {
            throw std::runtime_error("invalid symbol");
        }
    }
    if(temp_prefix.size() != 1){
        throw std::runtime_error("invalid regex");
    }
    return temp_prefix.top().first >= k;
}

bool Task_3_Solver::is_alphabet_symbol(char c) {
    return (c >= 'a' && c < 'a' + AlphabetSize) || c == '1';
}
