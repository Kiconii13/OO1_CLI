#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

// Struktura za prosleđivanje komandi

struct commandsFromInput {
    std::vector<std::vector<std::string>> commands;
    bool isPipeline = false;
    std::vector<bool> inputRedirected;
    std::vector<std::string> inputFile;
    std::vector<bool> outputRedirected;
    std::vector<std::string> outputFile;
    std::vector<bool> outputAppend;
};


// Pomoćne funkcije za rad. Ovo sam napravio kako bih izdvojio funckije za podelu inputa, tj razumevanja upisanog teksta
// Za razliku od interpretera (Koji služi za pokretanje i proveravanje već podeljenih komandi u tokene), utilities se samo bavi tekstom
// Utilities nije klasa, već skup funkcija. Da li je mogla da bude klasa? Mogla je, da li je morala da bude klasa? Nije, čim radi ovako :)
// Nadam se da pregledač neće uzeti za zlo implementiranje ovakvog pristupa - lakše mi je bilo da ovako podelim logičke celine programa
// umesto da sve što se tiče tumačenja komandi trpam u interpreter.

// Čitanje argumenta
std::string readArgument(const std::string& rawArgument);

// Čitanje sa ograničenjem od 512 karaktera
std::string readLimitedLine(size_t maxLength = 512);

// Podela stringa u numerabilni vektor
std::vector<std::string> splitString(const std::string& str);

// Podela inputa u numerabilni vektor, ako naiđe na tekst pod navodnicima, tretira ga kao jednu celinu
commandsFromInput splitInput(const std::string& input);

// Čitanje i provera da li je opcija u odgovarajućem formatu
std::string readOption(const std::string& rawOption);

// Provera da li postoje greške u unosu (znakovi poput {!.,*/-+...} na nedozvoljenim mestima)
std::string checkInput(const std::vector<std::string>& tokens);

#endif
