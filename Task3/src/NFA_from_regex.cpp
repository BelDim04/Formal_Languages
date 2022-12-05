#include "NFA_from_regex.h"


int NFA_from_regex::Vertex::symbol_to_int(char c) {
    return static_cast<int>(c - 'a');
}

void NFA_from_regex::Vertex::add_edge(char c, int vertex) {
    edges[symbol_to_int(c)].push_back(vertex);
}

void NFA_from_regex::Vertex::shift_indexing(int k) {
    for (int i = 0; i <= AlphabetSize; ++i) {
        for (auto& to_id : edges[i]) {
            to_id += k;
        }
    }
}

NFA_from_regex::NFA_from_regex() = default;

NFA_from_regex::NFA_from_regex(char c) : NFA_from_regex() {
    if (c == regex_epsilon) {
        final_vertex = 0;
    } else {
        vertices.emplace_back();
        final_vertex = 1;
        vertices.front().add_edge(c, 1);
    }
}

void NFA_from_regex::push_front_empty_vertex() {
    vertices.insert(vertices.begin(), Vertex());
    for (int i = 1; i < vertices.size(); ++i) {
        vertices[i].shift_indexing(1);
    }
    start_vertex += 1;
    final_vertex += 1;
}

NFA_from_regex& NFA_from_regex::operator+=(const NFA_from_regex& other) {
    push_front_empty_vertex();
    start_vertex = 0;
    vertices.front().add_edge(Epsilon, 1);
    vertices.front().add_edge(Epsilon, vertices.size());

    size_t old_sz = vertices.size();
    vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());
    for (int i = 0; i < other.vertices.size(); ++i) {
        vertices[i + old_sz].shift_indexing(old_sz);
    }

    vertices[final_vertex].add_edge(Epsilon, vertices.size());
    vertices[old_sz + other.final_vertex].add_edge(Epsilon, vertices.size());

    vertices.emplace_back();
    final_vertex = vertices.size() - 1;
    return *this;
}

NFA_from_regex& NFA_from_regex::operator*=(const NFA_from_regex& other) {
    size_t old_sz = vertices.size();
    vertices[final_vertex].add_edge(Epsilon, vertices.size());
    vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());

    for (int i = 0; i < other.vertices.size(); ++i) {
        vertices[i + old_sz].shift_indexing(old_sz);
    }
    final_vertex = old_sz + other.final_vertex;
    return *this;
}

void NFA_from_regex::make_closure() {
    push_front_empty_vertex();
    vertices.front().add_edge(Epsilon, 1);
    vertices[final_vertex].add_edge(Epsilon, 0);
    final_vertex = 0;
    start_vertex = 0;
}
