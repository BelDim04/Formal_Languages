#include <utility>
#include "src/Task_3_Solver.h"


int main(int argc, char** argv) {
    std::string s;
    char c;
    int k;
    std::cin >> s >> c >> k;
    std::cout << Task_3_Solver::task3(s, c, k) << "\n";
}


