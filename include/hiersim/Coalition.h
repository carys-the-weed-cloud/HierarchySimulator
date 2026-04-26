#pragma once
#include <string>
#include <vector>
#include <memory>

namespace hiersim {
class Organization;
class Policy;

struct Coalition {
    std::string id;
    std::string name;
    std::vector<std::string> memberOrgIds;
    std::string dominantIdeology;
    double stability = 1.0;
    
    void addMember(const std::string& orgId);
    void removeMember(const std::string& orgId);
};
}
