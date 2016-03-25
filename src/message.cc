#include <vector>
#include "message.h"
#include "protocol.h"

using namespace std;

Message::Message(int command, vector <MessageParam> parameters) : command(command), parameters(parameters) {

}

int Message::getType() {
    return command;
}

std::vector<MessageParam> Message::getParameters() {
    return parameters;
}
