#pragma once
#include <string>
#include <vector>

namespace hiersim {
class StringUtils {
public:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);
};
}
