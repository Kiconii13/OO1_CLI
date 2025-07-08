#define _CRT_SECURE_NO_WARNINGS
#include "Command.h"
#include "utilities.h"
#include <chrono>
#include <iomanip>
#include <fstream>



// Provera da li ima dovoljno tokena za rad komande // nasleđena metoda za sve komande

bool Command::validTokens(const std::vector<std::string>& tokens) {
    if (tokens.size() == getNumberOfTokens()) return true;
    std::cout << "Required number of arguments (including function name) for function " + tokens[0] + " is " + std::to_string(getNumberOfTokens()) + "\n";
    return false;
}


// EchoCommand setup

EchoCommand::EchoCommand(const std::string& arg) {
    if (arg.empty()) {
        // Čitanje sa standardnog ulaza ako je argument prazan
        std::ostringstream inputBuffer;
        std::string line;
        while (true) {
            if (std::getline(std::cin, line)) {
                inputBuffer << line << '\n';
            }
            // Ako je CTRL+Z (EOF) pritisnut ili je došlo do greške pri unosu, prekida unos
            if (std::cin.eof() || std::cin.fail()) {
                std::cin.clear();
                break;
            }

        }
        argument = inputBuffer.str();
    }
    else {
        argument = readArgument(arg);
    }
}


void EchoCommand::execute() {
    std::cout << argument << std::endl;
}

// TimeCommand setup

void TimeCommand::execute() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
    output = oss.str();

    std::cout << std::put_time(std::localtime(&in_time_t), "%H:%M:%S") << std::endl;
}



// DateCommand setup

void DateCommand::execute() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), "%d:%m:%Y");
    output = oss.str();

    std::cout << std::put_time(std::localtime(&in_time_t), "%d.%m.%Y") << std::endl;
}



//TouchCommand setup

void TouchCommand::execute(const std::string& fileName) {
    // Provera da li fajl postoji
    std::ifstream inFile(fileName);
    if (inFile) {
        std::cerr << "Error: File \"" << fileName << "\" already exists." << std::endl;
        return; // Izlaz bez kreiranja fajla
    }

    // Kreiranje praznog fajla
    std::ofstream outFile(fileName);
    if (outFile) {
        std::cout << "File \"" << fileName << "\" created successfully." << std::endl;
    }
    else {
        std::cerr << "Error: Could not create file \"" << fileName << "\"." << std::endl;
    }
}



// WordCountCommand setup

WordCountCommand::WordCountCommand(const std::string& opt, const std::string& arg) : option(readOption(opt)) {
    if (arg.empty()) {
        // Čitanje sa standardnog ulaza ako je argument prazan string
        std::ostringstream inputBuffer;
        std::string line;
        while (true) {
            if (std::getline(std::cin, line)) {
                inputBuffer << line << '\n';
            }
            // Ako je CTRL+Z (EOF) pritisnut ili je došlo do greške pri unosu, prekida unos
            if (std::cin.eof() || std::cin.fail()) {
                std::cin.clear();
                break;
            }
        }
        argument = inputBuffer.str();
    }
    else {
        argument = readArgument(arg);
    }
}

void WordCountCommand::execute() {
    if (option == "-w") {
        countWords();
    }
    else if (option == "-c") {
        countChars();
    }
    else {
        std::cerr << "Unknown option: " << option << std::endl;
    }
}

void WordCountCommand::countWords() {
    std::vector<std::string> words = splitString(argument);
    output = std::to_string(words.size());
    std::cout << words.size() << std::endl;
}

void WordCountCommand::countChars() {
    output = std::to_string(argument.size());
    std::cout << argument.size() << std::endl;
}


//TruncateCommand setup

void TruncateCommand::execute(const std::string& fileName) {
    // Provera da li fajl postoji
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error: File \"" << fileName << "\" doesn't exists." << std::endl;
        return; // Izlaz bez kreiranja fajla
    }

    // Kreiranje praznog fajla
    std::ofstream outFile(fileName);
    outFile.clear();
    std::cout << "File cleared!" << std::endl;
}

//RmCommand setup

void RmCommand::execute(const std::string& fileName) {
    // Provera da li fajl postoji
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error: File \"" << fileName << "\" doesn't exist." << std::endl;
        return; // Izlaz bez pokušaja brisanja
    }
    inFile.close(); // Zatvaranje fajla pre brisanja

    // Brisanje fajla
    int result = std::remove(fileName.c_str());
    if (result == 0) {
        std::cout << "File removed!" << std::endl;
    }
    else {
        std::cerr << "Error: File \"" << fileName << "\" could not be removed." << std::endl;
    }
}
// HeadCommand setup

HeadCommand::HeadCommand(const std::string& option, const std::string& arg) {
    // Provera da li opcija pocinje sa -n
    if (option.rfind("-n", 0) != 0 || option.size() <= 2) {
        throw std::runtime_error("Invalid option format. Use -n<count> with up to 5 digits.");
    }

    std::string numberPart = option.substr(2);
    if (numberPart.size() > 5 || numberPart.find_first_not_of("0123456789") != std::string::npos) {
        throw std::runtime_error("Invalid count format. Must be up to 5 digits.");
    }

    lineCount = std::stoi(numberPart);

    if (arg.empty()) {
        // Unos sa standardnog ulaza
        std::ostringstream inputBuffer;
        std::string line;
        while (std::getline(std::cin, line)) {
            inputBuffer << line << '\n';
        }
        std::cin.clear(); // Resetuje cin stanje nakon CTRL+Z
        content = inputBuffer.str();
    }
    else {
        content = readArgument(arg); // Tekst iz fajla ili stringa pod navodnicima
    }
}

void HeadCommand::execute() {
    std::istringstream iss(content);
    std::string line;
    int count = 0;
    while (count < lineCount && std::getline(iss, line)) {
        std::cout << line << '\n';
        !this->output.empty() ? this->output += line + '\n' : this->output = line + '\n';
        count++;
    }
}



TrCommand::TrCommand(const std::string& whatArg, const std::string& withArg, const std::string& arg)
    : what(readArgument(whatArg)), with(!withArg.empty() ? readArgument(withArg) : "")
{
    if (arg.empty()) {
        // Čitanje sa standardnog ulaza
        std::ostringstream inputBuffer;
        std::string line;
        while (std::getline(std::cin, line)) {
            inputBuffer << line << '\n';
        }
        argument = inputBuffer.str();
        std::cin.clear();
    }
    else {
        argument = readArgument(arg);
    }
}



void TrCommand::execute() {
    std::string result = argument;

    if (!what.empty()) {
        size_t pos = 0;
        while ((pos = result.find(what, pos)) != std::string::npos) {
            result.replace(pos, what.length(), with);
            pos += with.length();
        }
    }

    std::cout << result << std::endl;
    this->output = result;
}

bool TrCommand::validTokens(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1 && tokens.size() < 5) return true;
    std::cout << "Required format: tr [argument] what [with]\n";
    return false;
}