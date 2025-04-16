#include "sequence_tests.h"
#include <iostream>


int main() {
    try {
        run_sequence_tests("input.txt", "output.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}