#include "hiersim/Logger.h"
#include <iostream>
namespace hiersim {
Logger& Logger::instance() { static Logger l; return l; }
void Logger::log(LogLevel level, const std::string& message) {
    const char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    std::cout << "[" << levels[static_cast<int>(level)] << "] " << message << std::endl;
    if (file_.is_open()) file_ << "[" << levels[static_cast<int>(level)] << "] " << message << std::endl;
}
void Logger::setFile(const std::string& path) {
    if (file_.is_open()) file_.close();
    file_.open(path, std::ios::app);
}
}
