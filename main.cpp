#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){

    if (argc != 2) {
        std::cerr << "Incorrect usage" << std::endl;
        std::cerr << "hermes <input.hr>" << std::endl;
        return EXIT_FAILURE;
    }

    std::fstream input(argv[1], std::ios::in);

    std::cout << argv[1] << std::endl;
    return EXIT_SUCCESS;
}