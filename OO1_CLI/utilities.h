#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Pomoćne funkcije za rad

// Čitanje argumenta
std::string readArgument(const std::string& rawArgument);

// Čitanje sa ograničenjem od 512 karaktera
std::string readLimitedLine();

// Podela stringa u numerabilni vektor
std::vector<std::string> splitString(const std::string& str);

// Podela inputa u numerabilni vektor, ako naiđe na tekst pod navodnicima, tretira ga kao jednu celinu
std::vector<std::vector<std::string>> splitInput(const std::string& input);

// Čitanje i provera da li je opcija u odgovarajućem formatu
std::string readOption(const std::string& rawOption);

// Provera da li postoje greške u unosu (znakovi poput {!.,*/-+...} na nedozvoljenim mestima)
std::string checkInput(const std::vector<std::string>& tokens);

#endif
