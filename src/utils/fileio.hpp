#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <string>

#include <filesystem>

#include "utils.hpp"

namespace fileio {

    bool file_copy(const std::string &source_filename, const std::string &destination_filename) {
        std::ifstream source_file(source_filename, std::ios::binary);
        std::ofstream destination_file(destination_filename, std::ios::binary);
        if (!source_file.is_open()) {
            return false;
        }
        if (!destination_file.is_open()) {
            source_file.close();
            return false;
        }
        destination_file << source_file.rdbuf();
        source_file.close();
        destination_file.close();
        return true;
    }

    bool file_remove(const std::string &path) {
        const char *c_path = path.c_str();

        if (std::remove(c_path) != 0) {
            return false;
        }
        else {
            return true;
        }
    }

    bool write_utf8_file(const std::string &filename, const std::string &content) {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file << content;
            file.close();
            return true;
        }
        return false;
    }

    bool read_utf8_file(const std::string &filename, std::string &out) {
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        if (!file) {
            return false;
        }

        const size_t bufferSize = 4096;
        std::vector<char> buffer(bufferSize);
        std::string temp;

        while (file.read(buffer.data(), buffer.size())) {
            temp.append(buffer.data(), buffer.size());
        }

        temp.append(buffer.data(), file.gcount()); 
        out = temp; 

        file.close();
        return true;
    }

    bool write_file(const std::string &filename, const std::string &content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            return true;
        }
        
        return false;
    }

    bool read_file(const std::string &filepath, std::string &out) {
        std::ifstream file(filepath);
        if (!file) {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        out = buffer.str();
        return true; 
    }

    bool has_extension(const std::filesystem::path &filePath, const std::string &extension) {
        return filePath.extension() == extension;
    }

    bool file_exists(const std::string &path) {
        return std::filesystem::exists(path);
    }
}
