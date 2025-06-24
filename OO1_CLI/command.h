#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


// Apstraktna klasa komande
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {};

    // Provera da li je broj unetih tokena odgovarajuć funkciji koja je tražane
    bool validTokens(const std::vector<std::string>& numberOfTokens);
    inline static void validTokens(const int number) { std::cout << "Required number of arguments (including function name) is " << number << "\n"; }
    virtual inline int getNumberOfTokens() const { return numberOfTokens; }
private:
    int numberOfTokens = 1;
};

// Klasa komande za ispis teksta - echo
class EchoCommand : public Command {
public:
    explicit EchoCommand(const std::string& arg = "");
    void execute() override;
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    std::string argument;
    int numberOfTokens = 2;
};

// Klasa komande za ispis vremena - time
class TimeCommand : public Command {
public:
    void execute() override;
};

// Klasa komande za ispis datuma - date
class DateCommand : public Command {
public:
    void execute() override;
};

// Klasa komande za kreiranje fajla - touch 
class TouchCommand : public Command {
public:
    static void execute(const std::string& arg);
    inline static bool validTokens(const std::vector<std::string>& tokens) {
        if (tokens.size() == numberOfTokens) return true;
        std::cout << "Required number of arguments (including function name) for function \"touch\" is "
            << numberOfTokens << "\n";
        return false;
    }
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    static constexpr int numberOfTokens = 2;
};

// Klasa komande za prebrojavanje - wc
class WordCountCommand : public Command {
public:
    // Konstruktor koji prihvata argument i opciju komande, argumenti mogu biti -w i -c
    explicit WordCountCommand(const std::string& opt, const std::string& arg = "");
    
    void execute() override;

    // Metoda za prebrojavanje karaktera
    void countChars();
    
    // Metoda za prebrojavanje reči
    void countWords();

    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    std::string argument;
    std::string option;
    int numberOfTokens = 3;
};

// Klasa komande za brisanje sadržaja fajla - truncate
class TruncateCommand : public Command {
public:
    static void execute(const std::string& arg);
    inline static bool validTokens(const std::vector<std::string>& tokens) {
        if (tokens.size() == numberOfTokens) return true;
        std::cout << "Required number of arguments (including function name) for function \"truncate\" is "
            << numberOfTokens << "\n";
        return false;
    }
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    static constexpr int numberOfTokens = 2;
};

// Klasa komande za uklanjanje fajla - rm
class RmCommand : public Command {
public:
    static void execute(const std::string& arg);
    inline static bool validTokens(const std::vector<std::string>& tokens) {
        if (tokens.size() == numberOfTokens) return true;
        std::cout << "Required number of arguments (including function name) for function \"rm\" is "
            << numberOfTokens << "\n";
        return false;
    }
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    static constexpr int numberOfTokens = 2;
};
#endif
