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
    // Metoda za dobijanje prethodno ispisanog teksta
    inline std::string getLastOutput() const { return argument; };
private:
    std::string argument;
    int numberOfTokens = 2;
};

// Klasa komande za ispis vremena - time
class TimeCommand : public Command {
public:
    void execute() override;
    // Metoda za dobijanje prethodno ispisanog teksta
    inline std::string getLastOutput() const { return output; };
private:
    std::string output;
};

// Klasa komande za ispis datuma - date
class DateCommand : public Command {
public:
    void execute() override;
    // Metoda za dobijanje prethodno ispisanog teksta
    inline std::string getLastOutput() const { return output; };
private:
    std::string output;
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

    // Metoda za dobijanje prethodno ispisanog teksta
    inline int getLastOutput() const { return output; };

    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    std::string argument;
    std::string option;
    int output;
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

// Klasa komande za ispis prvih n linija - head
class HeadCommand : public Command {
public:
    HeadCommand(const std::string& option, const std::string& arg = "");
    void execute() override;

    int getNumberOfTokens() const override { return numberOfTokens; }
    static bool validTokens(const std::vector<std::string>& tokens);

private:
    static constexpr int numberOfTokens = 3;
    int lineCount;
    std::string content;
};

// Klasa komande za manipulaciju tekstom - tr
class TrCommand : public Command{
public:
    TrCommand(const std::string& whatArg, const std::string& withArg = "", const std::string& arg = "");
    void execute() override;
    int getNumberOfTokens() const override { return numberOfTokens; };
    static bool validTokens(const std::vector<std::string>& tokens);
    inline std::string getLastOutput() const { return output; };
    inline static void validTokens(const int number) { std::cout << "Required number of arguments (including function name) is " << number <<" or " << number+1 << "\n"; }
private:
    int numberOfTokens = 3;
    std::string output;
    std::string argument;
    std::string what;
    std::string with;
    bool replaceOnly;

};

#endif
