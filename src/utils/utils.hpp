#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include <windows.h>
#include <wincrypt.h>
#include <codecvt>
#include <regex>

#include "base64/base64.hpp"
#include "fileio.hpp"
#include <iomanip>

#include <cstdlib>
#include <ctime>

#include <chrono>

#include <filesystem>

#include <openssl/evp.h>
#include <filesystem>

namespace utils {

    namespace console {
        bool is_windows() {
#ifdef _WIN32
            return true;
#endif
            return false;
        }
    }

    namespace vector {

        void remove_duplicates(std::vector<std::string> &vec) {
            std::sort(vec.begin(), vec.end());
            vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
        }
    }

    namespace string {

        void safe_getenv(const std::string &name, std::string &out) {
            size_t requiredSize;
            char *buffer;

            getenv_s(&requiredSize, NULL, 0, name.c_str());

            buffer = new char[requiredSize];

            getenv_s(&requiredSize, buffer, requiredSize, name.c_str());

            out = std::string(buffer);
        }

        void match_regex(std::string content, const std::string &pattern, std::vector<std::string> &out) {
            std::regex regexPattern(pattern);
            std::smatch match;

            while (std::regex_search(content, match, regexPattern)) {
                out.push_back(match.str(1));
                content = match.suffix().str();
            }
        }
    }

    namespace conversion {

        void string_to_byte_vector(const std::string &in, std::vector<unsigned char> &out) {
            out = std::vector<unsigned char>(in.begin(), in.end());
        }

        void bytes_array_to_string(const unsigned char *in, size_t len, std::string &out) {
            out = std::string(reinterpret_cast<const char *>(in), len);
        }
    }
}