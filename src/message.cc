#include <vector>
#include "message.h"
#include "protocol.h"

using namespace std;

Message::Message(int command) : command(command) {


}

Message::Message(int command, vector <MessageParam> parameters) : command(command), parameters(parameters) {

}


int Message::getType() {
    return command;
}

std::vector<MessageParam> Message::getParameters() {
    return parameters;
}

void Message::add_param(MessageParam param) {
	parameters.push_back(param);
}

void Message::add_string_param(std::string value) {
	MessageParam param;
	param.requestType = Protocol::PAR_STRING;
	param.textValue = value;
	add_param(param);
}

void Message::add_number_param(int value) {
	MessageParam param;
	param.requestType = Protocol::PAR_NUM;
	param.numericValue = value;
	add_param(param);
}


