#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "arena.hpp"
#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"


int main(int argc, char* argv[]){
    const char* filename = argv[1];

    if(argc != 2){
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "main.o <input.bt>" << std::endl;
        return EXIT_FAILURE;
    }

    FILE* source = fopen(filename, "r");

    if(!source){
        std::cout << "couldn't open " << filename << std::endl;
    }

    std::stringstream file_stream;
    {
        std::fstream input(filename, std::ios::in);
        file_stream << input.rdbuf();
    }

    std::string content = file_stream.str();

    Tokenizer tokenizer(std::move(content));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeProgram> prog = parser.parse_program();

    if(!prog.has_value()){
        std::cerr << "Invalid program" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(prog.value());

    {
        std::fstream output_stream("out.asm", std::ios::out);
        output_stream << generator.gen_prog();   
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}