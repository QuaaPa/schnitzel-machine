
#include <cstdlib>
#include <exception>
#include <iostream>
#include "../include/core/core.hh"


int main(int argc, char *argv[]) {

#ifndef NDEBUG
    std::cout << "---- TOGGLE DEBUG MODE ----" << std::endl;
#endif
    
  
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
