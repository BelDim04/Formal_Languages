#include "FDFA.h"


FDFA::Vertex::Vertex() {
    edges.fill(-1);
}

size_t FDFA::size() {
    return vertices.size();
}

void FDFA::strip_by_class_and_symbol(std::vector<std::unordered_set<int>> &equivalence_classes,
                                     std::queue<int> &stripping_classes,
                                     std::unordered_set<int> &stripping_class,
                                     char symbol) {
    for (int equivalence_class_id = 0; equivalence_class_id < equivalence_classes.size(); ++equivalence_class_id) {
        auto &equivalence_class = equivalence_classes[equivalence_class_id];
        std::unordered_set<int> first_subclass, second_subclass;
        for (auto vertex_id: equivalence_class) {
            if (stripping_class.count(vertices[vertex_id].edges[symbol])) {
                first_subclass.insert(vertex_id);
            } else {
                second_subclass.insert(vertex_id);
            }
        }
        if (!first_subclass.empty() && !second_subclass.empty()) {
            equivalence_class = first_subclass;
            equivalence_classes.push_back(second_subclass);
            stripping_classes.push(equivalence_class_id);
            stripping_classes.push(equivalence_classes.size() - 1);
        }
    }
}

std::vector<std::unordered_set<int>> FDFA::get_equivalence_classes() {
    std::queue<int> stripping_classes;
    std::vector<std::unordered_set<int>> equivalence_classes(2);
    for (int vertex_id = 0; vertex_id < vertices.size(); ++vertex_id) {
        equivalence_classes[is_final[vertex_id]].insert(vertex_id);
    }
    if (equivalence_classes[1].empty()) {
        equivalence_classes.erase(equivalence_classes.begin() + 1);
    }
    if (equivalence_classes[0].empty()) {
        equivalence_classes.erase(equivalence_classes.begin());
    }
    stripping_classes.push(0);
    if (equivalence_classes.size() == 2) {
        stripping_classes.push(1);
    }

    while (!stripping_classes.empty()) {
        auto stripping_class = equivalence_classes[stripping_classes.front()];
        stripping_classes.pop();
        for (char symbol = 0; symbol < AlphabetSize; ++symbol) {
            strip_by_class_and_symbol(equivalence_classes, stripping_classes, stripping_class, symbol);
        }
    }
    return equivalence_classes;
}

void FDFA::minimize() {
    std::vector<std::unordered_set<int>> equivalence_classes = get_equivalence_classes();

    std::vector<int> class_by_vertex(vertices.size());
    for (int equivalence_class_id = 0; equivalence_class_id < equivalence_classes.size(); ++equivalence_class_id) {
        for (auto vertex_id: equivalence_classes[equivalence_class_id]) {
            class_by_vertex[vertex_id] = equivalence_class_id;
        }
    }

    // building new pdka by equivalence classes
    std::vector<Vertex> new_vertices(equivalence_classes.size());
    int new_start_vertex = 0;
    std::vector<uint8_t> new_is_final(equivalence_classes.size());
    for (int equivalence_class_id = 0; equivalence_class_id < equivalence_classes.size(); ++equivalence_class_id) {
        const auto &equivalence_class = equivalence_classes[equivalence_class_id];
        for (char symbol = 0; symbol < AlphabetSize; ++symbol) {
            int from_vertex = *equivalence_class.begin();
            int to_class = class_by_vertex[vertices[from_vertex].edges[symbol]];
            new_vertices[equivalence_class_id].edges[symbol] = to_class;
        }
        for (auto vertex_id: equivalence_class) {
            if (is_final[vertex_id]) {
                new_is_final[equivalence_class_id] = 1;
            }
        }
        if (equivalence_class.count(start_vertex)) {
            new_start_vertex = equivalence_class_id;
        }
    }
    std::swap(vertices, new_vertices);
    std::swap(start_vertex, new_start_vertex);
    std::swap(is_final, new_is_final);
}
