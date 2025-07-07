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
        this->findAndExecuteCommand(commands.commands[i],commands.isPipeline);

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

void Interpreter::checkAndExecuteTr(const std::vector<std::string>& tokens) {
    std::string arg;
    std::string what;
    std::string with;

    std::string lastOutput = !this->output.empty() ? this->output : "";

    if (tokens.size() > 3) {
        // tr argument what with
        arg = tokens[1];
        what = tokens[2];
        with = tokens[3];
    }
    else if (tokens.size() == 3) {
        if (!lastOutput.empty()) {
            // tr what with (pipeline scenario)
            arg = "\"" + lastOutput + "\"";
            what = tokens[1];
            with = tokens[2];
        }
        else {
            // tr argument what
            arg = tokens[1];
            what = tokens[2];
            with = "";
        }
    }
    else if (tokens.size() == 2) {
        // tr what
        if (!lastOutput.empty()) {
            arg = "\"" + lastOutput + "\"";
            what = tokens[1];
            with = "";
        }
        else {
            arg = "";
            what = tokens[1];
            with = "";
        }
    }
    else {
        std::cerr << "Invalid tr command syntax.\n";
        return;
    }

    TrCommand trCmd(what, with, arg);
    trCmd.execute();
    this->output = trCmd.getLastOutput();
}


void Interpreter::findAndExecuteCommand(std::vector<std::string>& command, bool isPipeline) {
    std::string lastOutput;

    if (isPipeline) {
        lastOutput = this->getLastOutput();
    }

    if (command.empty()) {
        std::cerr << "Empty command detected, skipping.\n";
        return;
    }

    std::string errorMessage = checkInput(command);

    if (!errorMessage.empty()) {
        std::cerr << errorMessage << std::endl;
        return;
    }

    // Komande kojima sme da se prosledi output iz prethodne komande
    auto injectLastOutputIfNeeded = [&](const std::string& cmdName) {
        if (!lastOutput.empty()) {
            command.push_back('"' + lastOutput + '"');
        }
        };



    if (command[0] == "echo") {
        if (isPipeline) injectLastOutputIfNeeded("echo");
        this->checkAndExecuteEcho(command);
    }
    else if (command[0] == "prompt") {
        if (isPipeline) injectLastOutputIfNeeded("prompt");
        this->checkAndExecutePrompt(command);
    }
    else if (command[0] == "time") {
        this->checkAndExecuteTime(command);
    }
    else if (command[0] == "date") {
        this->checkAndExecuteDate(command);
    }
    else if (command[0] == "touch") {
        this->checkAndExecuteTouch(command);
    }
    else if (command[0] == "wc") {
        if (isPipeline) injectLastOutputIfNeeded("wc");
        this->checkAndExecuteWC(command);
    }
    else if (command[0] == "truncate") {
        this->checkAndExecuteTruncate(command);
    }
    else if (command[0] == "rm") {
        this->checkAndExecuteRm(command);
    }
    else if (command[0] == "tr") {
        this->checkAndExecuteTr(command);
    }
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