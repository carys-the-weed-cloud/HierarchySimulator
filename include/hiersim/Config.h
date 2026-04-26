#pragma once
#include <string>
#include <unordered_map>

namespace hiersim {
class Config {
public:
    static Config& instance();
    void load(const std::string& path);
    void save(const std::string& path) const;
    std::string get(const std::string& key, const std::string& defaultVal = "") const;
    int getInt(const std::string& key, int defaultVal = 0) const;
    double getDouble(const std::string& key, double defaultVal = 0.0) const;
    bool getBool(const std::string& key, bool defaultVal = false) const;
    void set(const std::string& key, const std::string& value);
private:
    Config() = default;
    std::unordered_map<std::string, std::string> data_;
};
}
