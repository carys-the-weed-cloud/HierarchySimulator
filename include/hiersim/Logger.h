#pragma once
#include <string>
#include <fstream>

namespace hiersim {
enum class LogLevel { Debug, Info, Warning, Error };
class Logger {
public:
    static Logger& instance();
    void log(LogLevel level, const std::string& message);
    void setFile(const std::string& path);
private:
    Logger() = default;
    std::ofstream file_;
};
}
