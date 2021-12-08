#pragma once
#ifndef SBROWSER_FSTREAM_HPP
#define SBROWSER_FSTREAM_HPP

#include <fstream>
#include <ext/string.hpp>


ext::string file_read(const ext::string file_path) {
    std::string output;
    std::ifstream input(file_path);

    for (std::string file_line; std::getline(input, file_line);) output += line;
    return (ext::string)output;
}


#endif //SBROWSER_FSTREAM_HPP
