#pragma once
#include <string>
#include <unordered_map>

namespace hiersim {
class Ideology {
public:
    std::string name;
    double economicLeft = 0.5;
    double authoritarian = 0.5;
    double socialProgressive = 0.5;
    
    static Ideology capitalism();
    static Ideology socialism();
    static Ideology communism();
    static Ideology fascism();
    static Ideology anarchism();
    static Ideology liberalism();
    static Ideology conservatism();
};
}
