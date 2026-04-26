#include "hiersim/Ideology.h"
namespace hiersim {
Ideology Ideology::capitalism() { Ideology i; i.name = "Capitalism"; i.economicLeft = 0.2; i.authoritarian = 0.4; return i; }
Ideology Ideology::socialism() { Ideology i; i.name = "Socialism"; i.economicLeft = 0.8; i.authoritarian = 0.5; return i; }
Ideology Ideology::communism() { Ideology i; i.name = "Communism"; i.economicLeft = 1.0; i.authoritarian = 0.9; return i; }
Ideology Ideology::fascism() { Ideology i; i.name = "Fascism"; i.economicLeft = 0.3; i.authoritarian = 1.0; return i; }
Ideology Ideology::anarchism() { Ideology i; i.name = "Anarchism"; i.economicLeft = 0.7; i.authoritarian = 0.0; return i; }
Ideology Ideology::liberalism() { Ideology i; i.name = "Liberalism"; i.economicLeft = 0.4; i.authoritarian = 0.3; return i; }
Ideology Ideology::conservatism() { Ideology i; i.name = "Conservatism"; i.economicLeft = 0.5; i.authoritarian = 0.6; return i; }
}
