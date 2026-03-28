#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

enum class TokenType {
    _return, 
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value; 
}; 

int main(int argc, char* argv[]){

    if (argc != 2) {
        std::cerr << "Incorrect usage" << std::endl;
        std::cerr << "hermes <input.hr>" << std::endl;
        return EXIT_FAILURE;
    }

    
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::cout << contents << std::endl;
    

    return EXIT_SUCCESS;
}