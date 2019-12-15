#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class File {
public:
    static std::vector<std::string> loadLines(const char* path) {
        std::ifstream file(path);
        if (file.fail() || !file.is_open()) {
            std::cerr << "No such file: " << std::string(path) << std::endl;
        }

        std::vector<std::string> lines;
        std::string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    static std::string loadFile(const char* path) {
        std::ifstream file(path);
        if (file.fail() || !file.is_open()) {
            throw std::invalid_argument("No such file: " + std::string(path));
        }

        std::string source;
        std::string line;
        while (std::getline(file, line)) {
            source.append(line + "\n");
        }
        return std::string(source);
    }
};
