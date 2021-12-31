#pragma once
#ifndef SBROWSER_FILES_HPP
#define SBROWSER_FILES_HPP

#include <fstream>
#include <ext/iterables/string.hpp>


ext::string file_read(const ext::string file_path) {
    std::string output;
    std::ifstream input(file_path);

    for (std::string file_line; std::getline(input, file_line);) output += line;
    return (ext::string)output;
}


#endif //SBROWSER_FILES_HPP
