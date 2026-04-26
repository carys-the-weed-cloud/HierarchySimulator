#include "hiersim/Coalition.h"
namespace hiersim {
void Coalition::addMember(const std::string& orgId) {
    memberOrgIds.push_back(orgId);
}
void Coalition::removeMember(const std::string& orgId) {
    memberOrgIds.erase(std::remove(memberOrgIds.begin(), memberOrgIds.end(), orgId), memberOrgIds.end());
}
}
