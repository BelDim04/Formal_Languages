#include "Includes.h"
#include "NFA_from_regex.h"
#include "FDFA.h"

#ifndef AUTOMAT_NKA_H
#define AUTOMAT_NKA_H


class NFA {
    struct Vertex {
        std::array<std::bitset<BitsetSize>, AlphabetSize + 1> edges;
    };

public:
    int NKA_vertices_cnt = 0;
    std::vector<Vertex> vertices{static_cast<size_t>(NKA_vertices_cnt)};
    std::bitset<BitsetSize> is_start;
    std::bitset<BitsetSize> is_final;

    explicit NFA(const NFA_from_regex& nka);

    void epsilon_dfs(int start, std::vector<bool>& visited);

    void make_starts();

    void make_finals();

    void add_edges();

    void delete_epsilons();

    static void add_stock_vertex(FDFA& pdka);

    void handle_subset(std::unordered_map<std::bitset<BitsetSize>, int>& id,
                       std::queue<std::bitset<BitsetSize>>& subsets,
                       FDFA& pdka);

    FDFA to_PDKA();
};


#endif //AUTOMAT_NKA_H
