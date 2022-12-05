#include "NFA.h"


NFA::NFA(const NFA_from_regex &nka) : NKA_vertices_cnt(nka.vertices.size()) {
    for (int i = 0; i < NKA_vertices_cnt; ++i) {
        for (int c = 0; c <= AlphabetSize; ++c) {
            for (auto to_id: nka.vertices[i].edges[c]) {
                vertices[i].edges[c].set(to_id, true);
            }
        }
    }
    is_start[nka.start_vertex] = true;
    is_final[nka.final_vertex] = true;
}

void NFA::epsilon_dfs(int start, std::vector<bool> &visited) {
    visited[start] = 1;
    std::bitset<BitsetSize> &epsilon_edges_mask = vertices[start].edges.back();
    for (int to_id = epsilon_edges_mask._Find_first(); to_id < NKA_vertices_cnt; to_id = epsilon_edges_mask._Find_next(to_id)) {
        if (!visited[to_id]) {
            epsilon_dfs(to_id, visited);
        }
    }
}

void NFA::make_starts() {
    std::vector<bool> visited(NKA_vertices_cnt);
    epsilon_dfs(is_start._Find_first(), visited);
    for (int i = 0; i < NKA_vertices_cnt; ++i) {
        if (visited[i]) {
            is_start.set(i, true);
        }
    }
}

void NFA::make_finals() {
    int original_final = is_final._Find_first();
    for (int vertex = 0; vertex < NKA_vertices_cnt; ++vertex) {
        std::vector<bool> visited(NKA_vertices_cnt);
        epsilon_dfs(vertex, visited);
        if (visited[original_final]) {
            is_final.set(vertex, true);
        }
    }
}

void NFA::add_edges() {
    for (int vertex = 0; vertex < NKA_vertices_cnt; ++vertex) {
        std::vector<bool> visited(NKA_vertices_cnt);
        epsilon_dfs(vertex, visited);
        for (int to = 0; to < NKA_vertices_cnt; ++to) {
            if (visited[to]) {
                for (int c = 0; c < AlphabetSize; ++c) {
                    vertices[vertex].edges[c] |= vertices[to].edges[c];
                }
            }
        }
    }
}

void NFA::delete_epsilons() {
    add_edges();
    for (int vertex = 0; vertex < NKA_vertices_cnt; ++vertex) {
        vertices[vertex].edges.back() = 0;
    }
}

void NFA::add_stock_vertex(FDFA &pdka) {
    bool added_vertex = false;
    for (auto &vertex: pdka.vertices) {
        for (auto &to: vertex.edges) {
            if (to == -1) {
                to = pdka.vertices.size();
                added_vertex = true;
            }
        }
    }
    if (added_vertex) {
        pdka.vertices.emplace_back();
        pdka.vertices.back().edges.fill(pdka.vertices.size() - 1);
        pdka.is_final.push_back(0);
    }
}

void NFA::handle_subset(std::unordered_map<std::bitset<BitsetSize>, int> &id,
                        std::queue<std::bitset<BitsetSize>> &subsets,
                        FDFA &pdka) {
    std::bitset<BitsetSize> mask = subsets.front();
    subsets.pop();
    std::array<int, AlphabetSize> edges{};
    edges.fill(-1);
    for (int c = 0; c < AlphabetSize; ++c) {
        std::bitset<BitsetSize> to_mask;
        for (int vertex = mask._Find_first(); vertex < NKA_vertices_cnt; vertex = mask._Find_next(vertex)) {
            to_mask |= vertices[vertex].edges[c];
        }
        if (to_mask.any() && !id.count(to_mask)) {
            id.insert(std::make_pair(to_mask, id.size()));
            pdka.vertices.emplace_back();
            pdka.is_final.push_back((to_mask & is_final).any());
            subsets.push(to_mask);
        }
        if (to_mask.any()) {
            edges[c] = id[to_mask];
        }
    }
    pdka.vertices[id[mask]].edges = edges;
}

FDFA NFA::to_PDKA() {
    make_starts();
    make_finals();
    delete_epsilons();

    FDFA pdka;
    std::queue<std::bitset<BitsetSize>> subsets;
    subsets.push(is_start);
    std::unordered_map<std::bitset<BitsetSize>, int> id;
    id.max_load_factor(0.5);
    id[is_start] = 0;
    pdka.vertices.emplace_back();
    pdka.is_final.push_back((is_start & is_final).any());
    pdka.start_vertex = 0;
    while (!subsets.empty()) {
        handle_subset(id, subsets, pdka);
    }
    add_stock_vertex(pdka);
    return pdka;
}
