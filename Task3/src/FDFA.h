#include "Includes.h"

#ifndef AUTOMAT_PDKA_H
#define AUTOMAT_PDKA_H


class FDFA {
public:
    struct Vertex {
        std::array<int, AlphabetSize> edges{};
        Vertex();
    };

    size_t size();

    void strip_by_class_and_symbol(std::vector<std::unordered_set<int>>& equivalence_classes,
                                   std::queue<int>& stripping_classes,
                                   std::unordered_set<int>& stripping_class,
                                   char symbol);

    std::vector<std::unordered_set<int>> get_equivalence_classes();

    void minimize();

    std::vector<Vertex> vertices;
    int start_vertex = 0;
    std::vector<uint8_t> is_final;
};


#endif //AUTOMAT_PDKA_H
