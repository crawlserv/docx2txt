#include "docx2txt.hpp"

#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>

void PrintUsage(const std::string& name);
std::string ReadFile(const std::string& path);

int main(int argc, char ** argv) {
    if(argc < 3) {
        PrintUsage(argv[0]);

        return EXIT_FAILURE;
    }

    std::string inPath(argv[1]);
    std::string resultPath(argv[2]);

    if(inPath.empty() || resultPath.empty()) {
        std::cerr << "USAGE: " << argv[0] << " <input.docx> <expected.txt>" << std::endl;

        return EXIT_FAILURE;
    }

    try {
        const auto expected = ReadFile(resultPath);
        const auto actual = docx2txt::docx2txt(inPath);

        if(actual == expected) {
            return EXIT_SUCCESS;
        }

        std::cerr << '\'' << actual << "' <> '" << expected << '\'' << std::endl;

        return EXIT_FAILURE;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_FAILURE;
}

void PrintUsage(const std::string& name) {
    std::cerr << "USAGE: " << name << " <input.docx> <expected.txt>" << std::endl;
}

std::string ReadFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);

    if(!in.is_open()) {
        throw std::runtime_error(std::format("Could not open '{}'", path));
    }

    const auto size = std::filesystem::file_size(path);
    std::string line;
    std::string result;

    result.reserve(size);

    while(std::getline(in, line)) {
        result += line + "\n";
    }

    if(!result.empty()) {
        result.pop_back();
    }

    return result;
}