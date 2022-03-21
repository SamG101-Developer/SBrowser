#pragma once
#ifndef SBROWSER_FILES_HPP
#define SBROWSER_FILES_HPP

#include <fstream>

#include <ext/cpp_keywords.hpp>
#include <ext/string.hpp>


auto file_read(const ext::string& file_path) -> ext::string
{
    // create the input and output strings
    std::ifstream input(file_path.c_str());
    std::string output;

    // append each line of the file to the output
    for (std::string file_line; std::getline(input, file_line);)
        output += file_line;

    // return the output as an ext object
    return ext::string{output};
}

#endif //SBROWSER_FILES_HPP
