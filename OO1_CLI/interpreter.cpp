#include "interpreter.h"
#include "utilities.h"
#include <sstream>
#include <iostream>

void Interpreter::parseAndExecute(const std::string& line) {
    // Parsiranje unosa u komande
    commandsFromInput commands = splitInput(line);

    if (commands.commands.empty()) {
        std::cerr << "No function was called \n";
        return;
    }

    if (commands.isPipeline) {
        int commandIndex = 0;
    }

    // Iteracija kroz sve komande
    for (size_t i = 0; i < commands.commands.size(); i++) {
        if (commands.isPipeline)
            this->findCommandPipeline(commands.commands[i]);
        else
            this->findCommand(commands.commands[i]);

        // Ako postoji nešto u output promenljivoj
        if (!this->output.empty() && commands.outputRedirected[i]) {
            if (commands.outputAppend[i]) this->appendToFile(commands.outputFile[i], this->output);
            else this->overwriteFile(commands.outputFile[i], this->output);
        }
    }
    this->output.clear();
}

void Interpreter::changeSign(const std::string newSign) {
    sign = newSign + " ";
}

void Interpreter::checkAndExecuteEcho(const std::vector<std::string>& tokens) {
    try {
        EchoCommand echoCmd(tokens.size() > 1 ? tokens[1] : ""); // Ako nema argumenata, prosleđuje se prazan string
        if (tokens.size() <= 2) {
            echoCmd.execute();
            this->output = echoCmd.getLastOutput();
        }
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
        this->output = std::to_string(wc.getLastOutput());
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::checkAndExecuteTime(const std::vector<std::string>& tokens) {
    TimeCommand timeCommand;
    if (timeCommand.validTokens(tokens)) {
        timeCommand.execute();
        this->output = timeCommand.getLastOutput();
    }
}

void Interpreter::checkAndExecuteDate(const std::vector<std::string>& tokens) {
    DateCommand dateCommand;
    if (dateCommand.validTokens(tokens)) {
        dateCommand.execute();
        this->output = dateCommand.getLastOutput();
    }
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

void Interpreter::findCommand(const std::vector<std::string>& command) {
        if (command.empty()) {
            std::cerr << "Empty command detected, skipping.\n";
            return;
        }

        std::string errorMessage = checkInput(command);

        if (!errorMessage.empty()) {
            std::cerr << errorMessage << std::endl;
            return;
        }

        // echo command
        if (command[0] == "echo") {
            this->checkAndExecuteEcho(command);
        }

        // prompt command
        else if (command[0] == "prompt") {
            this->checkAndExecutePrompt(command);
        }

        // time command
        else if (command[0] == "time") {
            this->checkAndExecuteTime(command);
        }

        // date command
        else if (command[0] == "date") {
            this->checkAndExecuteDate(command);
        }

        // touch command
        else if (command[0] == "touch") {
            this->checkAndExecuteTouch(command);
        }

        // wc command
        else if (command[0] == "wc") {
            this->checkAndExecuteWC(command);
        }

        // truncate command
        else if (command[0] == "truncate") {
            this->checkAndExecuteTruncate(command);
        }

        // rm command
        else if (command[0] == "rm") {
            this->checkAndExecuteRm(command);
        }

        // undefined command
        else {
            std::cerr << "Unknown command: " << command[0] << std::endl;
        }
}

void Interpreter::findCommandPipeline(std::vector<std::string>& command) {
    std::string lastOutput = this->getLastOutput();

    if (command.empty()) {
        std::cerr << "Empty command detected, skipping.\n";
        return;
    }

    std::string errorMessage = checkInput(command);

    if (!errorMessage.empty()) {
        std::cerr << errorMessage << std::endl;
        return;
    }

    // echo command
    if (command[0] == "echo") {
        if (!lastOutput.empty()) command.push_back('"' + lastOutput + '"');
        this->checkAndExecuteEcho(command);
    }

    // prompt command
    else if (command[0] == "prompt") {
        if (!lastOutput.empty()) command.push_back('"' + lastOutput + '"');
        this->checkAndExecutePrompt(command);
    }

    // time command
    else if (command[0] == "time") {
        this->checkAndExecuteTime(command);
    }

    // date command
    else if (command[0] == "date") {
        this->checkAndExecuteDate(command);
    }

    // touch command
    else if (command[0] == "touch") {
        this->checkAndExecuteTouch(command);
    }

    // wc command
    else if (command[0] == "wc") {
        if (!lastOutput.empty()) command.push_back('"' + lastOutput + '"');
        this->checkAndExecuteWC(command);
    }

    // truncate command
    else if (command[0] == "truncate") {
        this->checkAndExecuteTruncate(command);
    }

    // rm command
    else if (command[0] == "rm") {
        this->checkAndExecuteRm(command);
    }

    // undefined command
    else {
        std::cerr << "Unknown command: " << command[0] << std::endl;
    }
}

void Interpreter::overwriteFile(const std::string& filename, const std::string& content) {
    std::ofstream fout(filename, std::ios::trunc);
    if (!fout.is_open()) {
        std::cerr << "Error opening file for overwrite: " << filename << std::endl;
        return;
    }
    fout << content;
    fout.close();
}

void Interpreter::appendToFile(const std::string& filename, const std::string& content) {
    std::ofstream fout(filename, std::ios::app);
    if (!fout.is_open()) {
        std::cerr << "Error opening file for append: " << filename << std::endl;
        return;
    }
    fout << content;
    fout.close();
}