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
    Message parse_next(const std::shared_ptr<Connection> &connection);
private:
    int read_number(const std::shared_ptr<Connection> &connection);
    std::string read_string(const std::shared_ptr<Connection> &connection);
};


#endif
