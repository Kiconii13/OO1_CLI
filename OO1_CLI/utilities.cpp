#include "utilities.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

// Funkcija za čitanje argumenata
// Koristi se string između navodnika
// Ukoliko nema navodnika, pretpostavlja se da je fajl
std::string readArgument(const std::string& rawArgument) {
    // Provera da li je argument pod navodnicima
    if (rawArgument.size() >= 2 && rawArgument.front() == '"' && rawArgument.back() == '"') {
        return rawArgument.substr(1, rawArgument.size() - 2);
    }
    // Ukoliko nije pod navodnicima, pretpostavlja se da je fajl
    else {
        std::ifstream file(rawArgument);
        // Ne može se pronaći fajl
        if (!file) {
            throw std::runtime_error("Error: Could not open file: " + rawArgument);
        }
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}

// Funkcija za čitanje linije do 512 znakova
std::string readLimitedLine() {
    std::string line;
    char ch;
    int count = 0;

    while (count < 512 && std::cin.get(ch) && ch != '\n') {
        line += ch;
        count++;
    }

    return line;
}


// Podela stringa po razmacima, smešteno u vektor
std::vector<std::string> splitString(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    // Čitanje svakog tokena i dodvanaje u vektor
    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}


// Provera unosa za nedozvoljene karaktere
std::string checkInput(const std::vector<std::string>& tokens) {
    // Regex za dozvoljenje karaktere van navodnika
    std::regex validFileTokenRegex(R"(^[a-zA-Z0-9\-._]+$)"); // Dozvoljava tačku van navodnika
    std::regex validCharacterRegex(R"(^[a-zA-Z0-9\-\"_]+$)"); // Ne -||-

    std::string fullInput;
    std::string errorPointer;

    bool inQuotes = false;      // on-off switch koji proverava da li smo unutar navodnika

    // Prolazak kroz sve tokene
    for (const auto& token : tokens) {
        fullInput += token + " ";

        for (size_t i = 0; i < token.size(); ++i) {
            char currentChar = token[i];

            // on-off
            if (currentChar == '"') {
                inQuotes = !inQuotes;
                errorPointer += ' ';
                continue;
            }

            // Unutar "" su svi znakovi validni
            if (inQuotes) {
                errorPointer += ' ';
                continue;
            }

            // Ako smo van navodnika proveravamo da li je tačka na dozvoljenom mestu
            if (currentChar == '.') {
                if (!std::regex_match(token, validFileTokenRegex)) {
                    errorPointer += '^'; // Greška za van navodnika
                }
                else {
                    errorPointer += ' '; // Dozvoljen unos
                }
            }
            else if (!std::regex_match(std::string(1, currentChar), validCharacterRegex)) {
                errorPointer += '^'; // Neispravan unos
            }
            else {
                errorPointer += ' '; // Ispravan znak
            }
        }

        // Razmak koji u donjoj liniji pokazuje da na zadatom karakteru nema greške
        errorPointer += ' ';
    }

    // Uklanjanje poslednjeg razmaka
    if (!fullInput.empty()) {
        fullInput.pop_back();
        errorPointer.pop_back();
    }

    // Provera da li su navodnici zatvoreni
    if (inQuotes) {
        return "Error - unmatched quotes in input.\n";
    }

    // Ako ne nađe '^' u stringu, znači da nema grešaka ^o^
    if (errorPointer.find('^') == std::string::npos) {
        return "";
    }

    // Generisanje poruke o grešci
    std::ostringstream errorMsg;
    errorMsg << "Error - unexpected characters:\n";
    errorMsg << fullInput << "\n";
    errorMsg << errorPointer;

    return errorMsg.str();
}

commandsFromInput splitInput(const std::string& input) {
    commandsFromInput commands;
    std::vector<std::string> currentCommand;
    std::istringstream stream(input);
    std::string token;
    int commandIndex = 0;
    bool inQuotes = false;
    char currentChar;
    bool outputFile = false;

    while (stream.get(currentChar)) {
        if (currentChar == '"') {
            token += currentChar;
            inQuotes = !inQuotes; // on/off za stanje da li je tekst pod navodnicima
            if (!inQuotes) {
                currentCommand.push_back(token);
                token.clear();
            }
        }
        else if (inQuotes) {
            token += currentChar; // Ubacuju se karakteri unutar navodnika
        }
        else if (currentChar == '|') {
            commands.isPipeline = true;
            if (!token.empty()) {
                currentCommand.push_back(token);
                token.clear();
            }
            if (!currentCommand.empty()) {
                commands.commands.push_back(currentCommand); // Dodavanje trenutne komande
                currentCommand.clear();
            }
        }
        else if (currentChar == '<');
        else if (currentChar == '>') {
            if (!commands.outputRedirected[commandIndex]) commands.outputRedirected[commandIndex] = true;
            else commands.outputAppend[commandIndex] = true;
        }
        else if (currentChar == ' ') {
            if (!token.empty()) {
                currentCommand.push_back(token); // Dodavanje kada se naidje na razmak
                token.clear();
            }
        }
        else {
            token += currentChar; // Ubacuju se karakteri van navodnika
        }
    }

    // Dodavanje poslednje komande ako postoji
    if (!token.empty()) {
        currentCommand.push_back(token);
    }
    if (!currentCommand.empty()) {
        commands.commands.push_back(currentCommand);
        commandIndex++;
    }

    return commands;
}


// Pomoćna funkcija koja je korišćena u testiranju
// Ostavljena u slučaju potrebe dalje provere opcija pri modifikaciji
std::string readOption(const std::string& rawOption) {
    return rawOption;
}