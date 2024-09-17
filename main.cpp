#include <iostream>
#include "Game/App.h"

int main() {
    appSpace::App app{};
    
    try{
        app.run();
    } catch (const std::exception &e){
        std::cerr <<  e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
