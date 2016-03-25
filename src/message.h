#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <vector>
#include <memory>
#include "connection.h"

/**
 * Container for message parameters
 */
struct MessageParam {
	// Added simple constructors to enable one line construction
	MessageParam() {}
	MessageParam(int rt, int numval, std::string text) : requestType(rt), numericValue(numval), textValue(text) {} 
    int requestType;
    int numericValue = 0;
    std::string textValue = "";
};

class Message {
public:
    Message(int command, std::vector<MessageParam> parameters);

    int getType();
    std::vector<MessageParam> getParameters();

private:
    int command;
    std::vector<MessageParam> parameters;
};


#endif
