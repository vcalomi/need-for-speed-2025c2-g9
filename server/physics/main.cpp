// main.cpp
#include <iostream>
#include "LevelCreator.h"

int main() {
    const char* kDir = "../server/physics/Levels"; 

    LevelCreator lc;
    lc.processDirectoryLevel(kDir);  // carga *_0..*_3.ldtkl

    const auto& levels = lc.levels();  // vector de 4 matrices
    for (int i = 0; i < 4; ++i) {
        if (levels[i].empty()) {
            std::cout << "nivel " << i << ": vacío\n";
        } else {
            std::cout << "nivel " << i << ": "
                      << levels[i].size() << "x" << levels[i][0].size()
                      << "  M[0][0]=" << levels[i][0][0] << "\n";
        }
    }
    return 0;
}