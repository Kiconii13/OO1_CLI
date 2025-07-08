#include <iostream>
#include "interpreter.h"
#include "utilities.h"

int main() {
    Interpreter& interpreter = Interpreter::getInstance();

    std::string line;
    std::cout << interpreter.getSign();

    while (true) {
        line = readLimitedLine();  // Čita liniju sa ograničenjima, max 512 karaktera
        
        // Ako naiđe na EOF, izlazi iz programa
        if (line.empty() && std::cin.eof()) {
            break;
        }

        interpreter.parseAndExecute(line);  // Interpeter interpetira
        std::cout << interpreter.getSign();  // Priprema za naredni komandu
    }

    std::cout << std::endl;
    return 0; // Rad programa se završio bez greške
}
