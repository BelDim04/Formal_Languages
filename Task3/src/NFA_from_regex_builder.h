#pragma once
#include "Includes.h"
#include "NFA_from_regex.h"

#ifndef AUTOMAT_NKA_FROM_REGEX_BUILDER_H
#define AUTOMAT_NKA_FROM_REGEX_BUILDER_H


class NFA_from_regex_builder {
    static bool is_alphabet_symbol(char c);

public:
    static NFA_from_regex build_NFA(const std::string& str);
};



#endif //AUTOMAT_NKA_FROM_REGEX_BUILDER_H
