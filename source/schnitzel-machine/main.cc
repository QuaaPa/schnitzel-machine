#include <cstdlib>

#include "core/core.h"

int main(int argc, char *argv[]) {      
    SM::Engine::getInstance()->run(argc, argv);

    return EXIT_SUCCESS;
}
