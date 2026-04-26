#pragma once
#include <string>

namespace hiersim {
class Policy {
public:
    std::string id;
    std::string name;
    std::string description;
    double economicImpact = 0.0;
    double socialImpact = 0.0;
    bool active = false;
};
}
