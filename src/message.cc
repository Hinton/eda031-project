#include <vector>
#include "message.h"

using namespace std;

Message::Message(int command, vector <MessageParameter> parameters) : command(command), parameters(parameters) {

}

int Message::getCommand() {
    return command;
}

std::vector<MessageParameter> Message::getParameters() {
    return parameters;
}


