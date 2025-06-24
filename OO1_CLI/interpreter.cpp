#include "interpreter.h"
#include "utilities.h"
#include <sstream>
#include <iostream>

void Interpreter::parseAndExecute(const std::string& line) {
    // Parsiranje unosa u komande
    std::vector<std::vector<std::string>> commands = splitInput(line);

    if (commands.empty()) {
        std::cerr << "No function was called \n";
        return;
    }

    // Iteracija kroz sve komande
    for (const auto& tokens : commands) {
        if (tokens.empty()) {
            std::cerr << "Empty command detected, skipping.\n";
            continue;
        }

        std::string errorMessage = checkInput(tokens);

        if (!errorMessage.empty()) {
            std::cerr << errorMessage << std::endl;
            continue;
        }

        // echo command
        if (tokens[0] == "echo") {
            this->checkAndExecuteEcho(tokens);
        }

        // prompt command
        else if (tokens[0] == "prompt") {
            this->checkAndExecutePrompt(tokens);
        }

        // time command
        else if (tokens[0] == "time") {
            this->checkAndExecuteTime(tokens);
        }

        // date command
        else if (tokens[0] == "date") {
            this->checkAndExecuteDate(tokens);
        }

        // touch command
        else if (tokens[0] == "touch") {
            this->checkAndExecuteTouch(tokens);
        }

        // wc command
        else if (tokens[0] == "wc") {
            this->checkAndExecuteWC(tokens);
        }

        // truncate command
        else if (tokens[0] == "truncate") {
            this->checkAndExecuteTruncate(tokens);
        }

        // rm command
        else if (tokens[0] == "rm") {
            this->checkAndExecuteRm(tokens);
        }

        // undefined command
        else {
            std::cerr << "Unknown command: " << tokens[0] << std::endl;
        }
    }
}

void Interpreter::changeSign(const std::string newSign) {
    sign = newSign + " ";
}

void Interpreter::checkAndExecuteEcho(const std::vector<std::string>& tokens) {
    try {
        EchoCommand echoCmd(tokens.size() > 1 ? tokens[1] : ""); // Ako nema argumenata, prosleđuje se prazan string
        if (tokens.size() <= 2) echoCmd.execute();
        else Command::validTokens(2);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::checkAndExecutePrompt(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cerr << "Error: Missing argument for prompt command. Expected 1 argument.\n";
        return;
    }

    try {
        std::string argument = readArgument(tokens[1]);
        if (tokens.size() == 2) this->changeSign(argument);
        else std::cout << "Required number of arguments (including function name) for function "
            << tokens[0] << " is 2\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::checkAndExecuteTouch(const std::vector<std::string>& tokens) {
    try {
        if (TouchCommand::validTokens(tokens)) TouchCommand::execute(tokens[1]);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::checkAndExecuteWC(const std::vector<std::string>& tokens) {
    try {
        if (tokens.size() < 2 || (tokens[1] != "-w" && tokens[1] != "-c")) {
            std::cerr << "Error: Missing option (-w or -c) for wc command.\n";
            return;
        }
        if (tokens.size() > 3) {
            Command::validTokens(3);
            return;
        }
        WordCountCommand wc(tokens[1], tokens.size() > 2 ? tokens[2] : ""); // Ako nema argumenta, aktiviraj unos sa komandne linije
        wc.execute();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::checkAndExecuteTime(const std::vector<std::string>& tokens) {
    TimeCommand timeCommand;
    if (timeCommand.validTokens(tokens)) timeCommand.execute();
}

void Interpreter::checkAndExecuteDate(const std::vector<std::string>& tokens) {
    DateCommand dateCommand;
    if (dateCommand.validTokens(tokens)) dateCommand.execute();
}

void Interpreter::checkAndExecuteTruncate(const std::vector<std::string>& tokens) {
    try {
        if (TruncateCommand::validTokens(tokens)) TruncateCommand::execute(tokens[1]);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::checkAndExecuteRm(const std::vector<std::string>& tokens) {
    try {
        if (RmCommand::validTokens(tokens)) RmCommand::execute(tokens[1]);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}
