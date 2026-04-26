#pragma once
#include <string>

namespace hiersim {
struct Location {
    std::string id;
    double latitude = 0.0;
    double longitude = 0.0;
    std::string regionId;
};
}
