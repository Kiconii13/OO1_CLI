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
    virtual std::string getLastOutput() const { return ""; }
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

    // Izvršava komandu wc
    void execute() override;

    // Metoda za prebrojavanje karaktera
    void countChars();

    // Metoda za prebrojavanje reči
    void countWords();

    // Metoda za dobijanje prethodno ispisanog teksta
    inline std::string getLastOutput() const { return output; };

    // Vraća koji je potreban broj tokena za izvršavanje funkcije
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    std::string argument;
    std::string option;
    std::string output;
    int numberOfTokens = 3;
};

// Klasa komande za brisanje sadržaja fajla - truncate
class TruncateCommand : public Command {
public:
    // Izvršava truncate komandu
    static void execute(const std::string& arg);
    // Proverava da li je broj tokena ispravan - ukoliko nije, ispisaće potreban broj tokena
    inline static bool validTokens(const std::vector<std::string>& tokens) {
        if (tokens.size() == numberOfTokens) return true;
        std::cout << "Required number of arguments (including function name) for function \"truncate\" is "
            << numberOfTokens << "\n";
        return false;
    }
    // Vraća broj tokena potreban za izvršenje funkcije
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    static constexpr int numberOfTokens = 2;
};

// Klasa komande za uklanjanje fajla - rm
class RmCommand : public Command {
public:
    // Izvršava komandu rm
    static void execute(const std::string& arg);
    // Proverava da li je broj tokena ispravan - ukoliko nije, ispisaće potreban broj tokena
    inline static bool validTokens(const std::vector<std::string>& tokens) {
        if (tokens.size() == numberOfTokens) return true;
        std::cout << "Required number of arguments (including function name) for function \"rm\" is "
            << numberOfTokens << "\n";
        return false;
    }
    // Vraća broj tokena potreban za izvršenje funkcije
    int getNumberOfTokens() const override { return numberOfTokens; }
private:
    static constexpr int numberOfTokens = 2;
};

// Klasa komande za ispis prvih n linija - head
class HeadCommand : public Command {
public:
    // Konstruktor komande - option je u formatu -n[number] i time prepoznaje koliko linija treba da ispiše
    // arg je tekst koji treba da obradi / ispiše, ukoliko nije dat, default vrednost je "" što će interpreter
    // prepoznati kao da treba da čita sa komandne linije
    HeadCommand(const std::string& option, const std::string& arg = "");
    // Izvršava komandu
    void execute() override;
    
    // Vraća potreban broj tokena
    int getNumberOfTokens() const override { return numberOfTokens; }
    // Proverava da li su tokeni validni
    inline static bool validTokens(const std::vector<std::string>& tokens) {
        if (tokens.size() == 3) return true; // head -nX "tekst" ili fajl
        if (tokens.size() == 2) return true; // head -nX  (standardni ulaz)
        std::cout << "Required format: head -n<count> [argument]\n";
        return false;
    }
    // Vraća poslednje tekst koji je poslednji ispisan
    inline std::string getLastOutput() const { return output; };

private:
    static constexpr int numberOfTokens = 3;
    std::string output;
    int lineCount;
    std::string content;
};

// Klasa komande za manipulaciju tekstom - tr
class TrCommand : public Command{
public:
    // Konstruktor za komandu tr - what argument je obavezan i označava šta treba promeniti u tekstu koji će se zadati
    // withArg zadaje informaciju čime treba zameniti ukoliko se u tekstu pronađe *what*. Ukoliko se ne zada u pozivu konstruktora
    // default vrednost je "" što će interpreter pročitiati kao da treba da obriše *what* u tekstu
    // arg je tekst nad kojim se manipuliše - ukoliko nije zadat i funkcija nema prethodno korišćen tekst iz pipeline-a, čita sa ulaznog toka
    TrCommand(const std::string& whatArg, const std::string& withArg = "", const std::string& arg = "");
    // Izvršava komandu
    void execute() override;
    // Vraća broj tokena potreban za pozivanje funkcije
    int getNumberOfTokens() const override { return numberOfTokens; };
    // Proverava da li su tokeni ispravni
    static bool validTokens(const std::vector<std::string>& tokens);
    // Vraća poslednji ispis na komandnoj liniji
    inline std::string getLastOutput() const { return output; };
private:
    int numberOfTokens = 2;
    std::string output;
    std::string argument;
    std::string what;
    std::string with;
    bool replaceOnly;

};

#endif
