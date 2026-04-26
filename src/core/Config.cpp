#include "hiersim/Config.h"
#include <fstream>
namespace hiersim {
Config& Config::instance() { static Config c; return c; }
void Config::load(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) data_[line.substr(0, pos)] = line.substr(pos + 1);
    }
}
void Config::save(const std::string& path) const {
    std::ofstream file(path);
    for (const auto& [k, v] : data_) file << k << "=" << v << "\n";
}
std::string Config::get(const std::string& key, const std::string& defaultVal) const {
    auto it = data_.find(key); return (it != data_.end()) ? it->second : defaultVal;
}
int Config::getInt(const std::string& key, int defaultVal) const {
    auto it = data_.find(key); return (it != data_.end()) ? std::stoi(it->second) : defaultVal;
}
double Config::getDouble(const std::string& key, double defaultVal) const {
    auto it = data_.find(key); return (it != data_.end()) ? std::stod(it->second) : defaultVal;
}
bool Config::getBool(const std::string& key, bool defaultVal) const {
    auto it = data_.find(key); return (it != data_.end()) ? (it->second == "true") : defaultVal;
}
void Config::set(const std::string& key, const std::string& value) { data_[key] = value; }
}
