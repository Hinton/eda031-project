#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <iostream>
#include <memory>
#include "protocol.h"
#include "connection.h"
#include "message.h"

/**
 * Parser for reading messages on a connection.
 */
class MessageHandler {
public:
    MessageHandler();

    /**
     * Parse the next message on the connection.
     */
    Message parse_message(const std::shared_ptr<Connection> &connection);
    void send_message(const std::shared_ptr<Connection> &connection, Message message);

private:
    int read_number(const std::shared_ptr<Connection> &connection);
    std::string read_string(const std::shared_ptr<Connection> &connection);

    void write_number(const std::shared_ptr<Connection> & connection, int value);
    void write_string(const std::shared_ptr<Connection> & connetion, std::string value);
};


#endif
