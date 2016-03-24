#include <vector>
#include "command.h"

using namespace std;

Command::Command(int command, vector <CommandParam> parameters) : command(command), parameters(parameters) {

}

int Command::getCommand() {
    return command;
}

std::vector<CommandParam> Command::getParameters() {
    return parameters;
}


