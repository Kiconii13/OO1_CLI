#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Struktura za prosleđivanje komandi

struct commandsFromInput {
    std::vector<std::vector<std::string>> commands;
    bool isPipeline = false;
    std::vector<bool> inputRedirected;
    std::vector<bool> outputRedirected;
    std::vector<std::string> outputFile;
    std::vector<bool> outputAppend;
};


// Pomoćne funkcije za rad

// Čitanje argumenta
std::string readArgument(const std::string& rawArgument);

// Čitanje sa ograničenjem od 512 karaktera
std::string readLimitedLine();

// Podela stringa u numerabilni vektor
std::vector<std::string> splitString(const std::string& str);

// Podela inputa u numerabilni vektor, ako naiđe na tekst pod navodnicima, tretira ga kao jednu celinu
commandsFromInput splitInput(const std::string& input);

// Čitanje i provera da li je opcija u odgovarajućem formatu
std::string readOption(const std::string& rawOption);

// Provera da li postoje greške u unosu (znakovi poput {!.,*/-+...} na nedozvoljenim mestima)
std::string checkInput(const std::vector<std::string>& tokens);

#endif
