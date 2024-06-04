/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** parsing
*/

#pragma once

#include "../../lib_headers/common.hpp"
#include "Exception.hpp"
#include <iomanip>
#include <sstream>
#include <fstream>
#include "../math/Vector2.hpp"

namespace ypi {
    namespace parsing {
        static std::string removeExtension(const std::string& filename) {
            size_t lastdot = filename.find_last_of(".");
            if (lastdot == std::string::npos) return filename;
            return filename.substr(0, lastdot);
        }

        static std::string getFileContent(const std::string& path) {
            std::ifstream file(path);
            std::string content;
            std::string line;
            if (!file.is_open())
                THROW("Failed to open file: " + path);
            while (std::getline(file, line)) {
                content += line;
                content.push_back('\n');
            }
            return content;
        }

        template<typename T>
        static std::string Vec2Str(const Vector2<T>& vec) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << vec.x << ", " << vec.y;
            return ss.str();
        }

        template<typename T>
        static Vector2<T> Str2Vec2(const std::string& str) {
            std::string tmp = str;
            std::stringstream ss(tmp);
            std::string token;
            std::vector<std::string> tokens;

            // remove spaces
            tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());

            while (std::getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() != 2)
                THROW("Invalid vector format: " + tmp);
            return Vector2<T>(std::stof(tokens[0]), std::stof(tokens[1]));
        }

        static std::string floatToString(float f, int precision = 1) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision) << f;
            return ss.str();
        }

        static bool isUnsignedInt(char c) { return c >= '0' && c <= '9'; }
        static bool isInt(char c) { return isUnsignedInt(c) || c == '-'; }
        static bool isFloat(char c) { return isInt(c) || c == '.'; }
        static bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
        static bool isAlphaNum(char c) { return isAlpha(c) || isInt(c); }
    }
}


