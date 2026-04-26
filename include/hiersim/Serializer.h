#pragma once
#include <string>
#include <sstream>

namespace hiersim {
class Serializer {
public:
    template<typename T>
    static std::string serialize(const T& obj);
    template<typename T>
    static T deserialize(const std::string& data);
};
}
