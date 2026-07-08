#include <cstdlib>
#include <exception>
#include <iostream>

#include "core/core.h"

int main(int argc, char *argv[]) {      
    Engine &engine = *Engine::getInstance();

    try {
        engine.run(argc, argv);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;      
    }

    return EXIT_SUCCESS;
}
