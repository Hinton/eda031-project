#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <vector>
#include "connection.h"

/**
 * Container for message parameters
 */
struct MessageParam {
    int requestType;
    int numericValue = 0;
    std::string textValue = "";
};

class Message {
public:
    Message(int command, std::vector<MessageParam> parameters);

    int getType();
    std::vector<MessageParam> getParameters();

    void send(const std::shared_ptr<Connection> &connection);

private:
    int command;
    std::vector<MessageParam> parameters;

    void write_number(const std::shared_ptr<Connection> & connection, int value);
    void write_string(const std::shared_ptr<Connection> & connetion, std::string value);
};


#endif
