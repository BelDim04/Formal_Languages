#include "NFA_from_regex_builder.h"


NFA_from_regex NFA_from_regex_builder::build_NFA(const std::string& str) {
    std::stack<NFA_from_regex> temp_nfas;
    int i = 0;
    for (auto c: str) {
        if (is_alphabet_symbol(c)) {
            temp_nfas.emplace(c);
        } else if (c == '.' || c == '+') {
            if(temp_nfas.size() < 2) {
                throw std::runtime_error("invalid regex");
            }
            NFA_from_regex b = std::move(temp_nfas.top());
            temp_nfas.pop();
            NFA_from_regex a = std::move(temp_nfas.top());
            temp_nfas.pop();
            if (c == '+') {
                temp_nfas.push(std::move(a+=b));
            }
            else {
                temp_nfas.push(std::move(a*=b));
            }
        } else if (c == '*') {
            temp_nfas.top().make_closure();
        } else {
            throw std::runtime_error("invalid symbol");
        }
        ++i;
        //cout << i << '\n';
    }
    if(temp_nfas.size() != 1){
        throw std::runtime_error("invalid regex");
    }
    return temp_nfas.top();
}

bool NFA_from_regex_builder::is_alphabet_symbol(char c) {
    return (c >= 'a' && c < 'a' + AlphabetSize) || c == '1';
}
