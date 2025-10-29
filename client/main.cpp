#include <exception>
#include <iostream>

#include "./core/game.h"

int main() try {
    Game game;
    game.Run();
    return 0;

} catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
