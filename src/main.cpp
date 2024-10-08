#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "tokenization.h"
#include "parser.h"
#include "generation.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect Usage. Correct usage is..." << std::endl;
        std::cerr << "Hydrogen <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::ifstream input(argv[1]);
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<node::Program> program = parser.parse_program();

    if (!program.has_value()) throw std::runtime_error("Invalid program...");
    {
        Generator generator(program.value());
        std::fstream file("out.asm", std::ios::out);
        file << generator.generate_program();
    }

    system("as -o out.o out.asm");
    system("clang -o out out.o");


    return EXIT_SUCCESS;
}
