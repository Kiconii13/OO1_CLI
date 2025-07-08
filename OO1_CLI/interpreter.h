#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <memory>
#include "Command.h"

class Interpreter {
public:
    // Statička metoda za pristup instanci singletona
    static Interpreter& getInstance() {
        static Interpreter instance; // Stvara instancu samo pri prvom pozivu
        return instance;
    }

    // Zabranjujemo kopiranje i premeštanje instance
    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;
    Interpreter(Interpreter&&) = delete;
    Interpreter& operator=(Interpreter&&) = delete;


    // Glavna metoda za izvršavanje zadatih komandi
    void parseAndExecute(const std::string& line);

    // Prompt sign getter
    inline std::string getSign() { return sign; }

    // Metoda za menjanje prompt znaka - de facto sign setter
    void changeSign(const std::string newSign);

    // Metoda za pronalazak komande i pokretanje izvršavanja
    void findAndExecuteCommand(std::vector<std::string>& command, bool isPipeline, const std::string& inputFile = "");

    // Metoda za proveru i izvršenje echo komande unutar interpetera
    void checkAndExecuteEcho(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje prompt komande unutar interpetera
    void checkAndExecutePrompt(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje touch komande unutar interpetera
    void checkAndExecuteTouch(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje wc komande unutar interpetera
    void checkAndExecuteWC(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje time komande unutar interpetera
    void checkAndExecuteTime(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje date komande unutar interpetera
    void checkAndExecuteDate(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje truncate komande unutar interpretera
    void checkAndExecuteTruncate(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje rm komande unutar interpretera
    void checkAndExecuteRm(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje tr komande unutar interpretera
    void checkAndExecuteTr(const std::vector<std::string>& tokens, const std::string& lastOutput, const std::string& fileContent);

    // Metoda za proveru i izvršenje batch komande unutar interpretera
    void checkAndExecuteBatch(const std::vector<std::string>& tokens);

    // Metoda za proveru i izvršenje head komande unutar interpretera
    void checkAndExecuteHead(const std::vector<std::string>& tokens);

    // Izvršava redirekciju - upis u fajl
    void overwriteFile(const std::string& filename, const std::string& content);

    // Izvršava redirekciju - dodajte na postojeći sadržaj fajla
    void appendToFile(const std::string& filename, const std::string& content);

    // Metoda za vraćanje poslednjeg ispisanog reda na komandnoj liniji
    inline std::string getLastOutput() const { return output; }; 

private:

    // Privatni konstruktor za sprečavanje instanciranja
    Interpreter() = default;

    // Poslednji ispisan tekst na komandnoj liniji
    std::string output;

    // Privatni destruktor
    ~Interpreter() = default;

    // Prompt znak, defalut je $
    std::string sign = "$ ";
};

#endif 
