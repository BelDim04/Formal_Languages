#include "Includes.h"

#ifndef AUTOMAT_NKA_FROM_REGEX_H
#define AUTOMAT_NKA_FROM_REGEX_H


class NFA_from_regex { // has exactly 1 final vertex
    struct Vertex {
        std::array<std::vector<int>, AlphabetSize + 1> edges;

        static int symbol_to_int(char c);
        void add_edge(char c, int vertex);
        void shift_indexing(int k);
    };

    friend class NFA_from_regex_builder;
    friend class NFA;

    static const char regex_epsilon = '1';

    std::vector<Vertex> vertices{Vertex()};
    int final_vertex = 0;
    int start_vertex = 0;

    NFA_from_regex();

    void push_front_empty_vertex();

    NFA_from_regex& operator+=(const NFA_from_regex& other);

    NFA_from_regex& operator*=(const NFA_from_regex& other);

    void make_closure();

public:
    explicit NFA_from_regex(char c);

};



#endif //AUTOMAT_NKA_FROM_REGEX_H
