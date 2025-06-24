#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <memory>
#include "Command.h"

class Interpreter {
public:
    // Funkcija za pristup instanci singletona
    static Interpreter& getInstance() {
        static Interpreter instance; // Stvara instancu samo pri prvom pozivu
        return instance;
    }

    // Zabranjujemo kopiranje i premeštanje instance
    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;
    Interpreter(Interpreter&&) = delete;
    Interpreter& operator=(Interpreter&&) = delete;


    // Funkcija za izvršavanje zadatih komandi
    void parseAndExecute(const std::string& line);
    
    // Prompt sign getter
    inline std::string getSign() { return sign; }
    
    // Funkcija za menjanje prompt znaka
    void changeSign(const std::string newSign);

    // Funkcija za izvršenje echo komande unutar interpetera
    void checkAndExecuteEcho(const std::vector<std::string>& tokens);

    // Funkcija za izvršenje prompt komande unutar interpetera
    void checkAndExecutePrompt(const std::vector<std::string>& tokens);

    // Funkcija za izvršenje touch komande unutar interpetera
    void checkAndExecuteTouch(const std::vector<std::string>& tokens);

    // Funkcija za izvršenje wc komande unutar interpetera
    void checkAndExecuteWC(const std::vector<std::string>& tokens);

    // Funkcija za izvršenje time komande unutar interpetera
    void checkAndExecuteTime(const std::vector<std::string>& tokens);

    // Funkcija za izvršenje date komande unutar interpetera
    void checkAndExecuteDate(const std::vector<std::string>& tokens);

    // Funkcija za izvršavanje truncate komande unutar interpretera
    void checkAndExecuteTruncate(const std::vector<std::string>& tokens);

    // Funkcija za izvršavanje rm komande unutar interpretera
    void checkAndExecuteRm(const std::vector<std::string>& tokens);
private:

    // Privatni konstruktor za sprečavanje instanciranja
    Interpreter() = default;

    // Privatni destruktor
    ~Interpreter() = default;

    // Prompt znak, defalut je $
    std::string sign = "$ ";
};

#endif 
