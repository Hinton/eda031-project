#include "messagehandler.h"

using namespace std;

MessageHandler::MessageHandler() {

}

// TODO: Throw errors on parsing issues?
Message MessageHandler::parse_message(const std::shared_ptr<Connection> &connection) {

    // Get the command;
    int command = connection->read();

    std::cout << "COMMAND: " << command << endl;

    int type = connection->read();

	Message message(command);

    // Continue untill the message is finnished
    while (type != Protocol::COM_END && type != Protocol::ANS_END) {

        cout << "TYPE: " << type << endl;

        MessageParam param;
        param.requestType = type;

        if (type == Protocol::PAR_NUM) {
            param.numericValue = read_number(connection);
        } else if (type == Protocol::PAR_STRING) {
            param.textValue = read_string(connection);
        } else if (type == Protocol::ANS_ACK) {
            // No need to do anything on ack.
        } else if (type == Protocol::ANS_NAK) {
            // Load the error message.
            param.numericValue = connection->read();
        } else {
            cerr << "UNKNOWN PARAMETER TYPE " << param.requestType << endl;
        }

        cout << "PARAM: " << endl;
        cout << param.requestType << " " << param.numericValue << " " << param.textValue << endl;

        message.add_param(param);

        type = connection->read();
    }

    return message;
}

/**
 * Int consists of 4 bytes.
 */
int MessageHandler::read_number(const std::shared_ptr<Connection> &connection) {
    unsigned char byte1 = connection->read();
    unsigned char byte2 = connection->read();
    unsigned char byte3 = connection->read();
    unsigned char byte4 = connection->read();

    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/**
 * [ N (length), char1, char2, ...  charN ]
 */
std::string MessageHandler::read_string(const std::shared_ptr<Connection> &connection) {
    // How long is the string?
    int length = read_number(connection);

    string s;
    for (int i = 0; i != length; ++i) {
        s += connection->read();
    }

    return s;
}


void MessageHandler::send_message(const shared_ptr<Connection> &connection, Message message) {

    auto command = message.getType();

    // Send Protocol::<cmd>
    unsigned char cmd = static_cast<unsigned char>(command);
    connection->write(cmd);

    // Send parameters
    auto parameters = message.getParameters();
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {

        MessageParam p = *it;

        switch (p.requestType) {

            case Protocol::PAR_NUM:
                connection->write(Protocol::PAR_NUM);
                write_number(connection, p.numericValue);
                break;
            case Protocol::PAR_STRING:
                connection->write(Protocol::PAR_STRING);
                write_string(connection, p.textValue);
                break;
            case Protocol::ANS_ACK:
                connection->write(Protocol::ANS_ACK);
                break;
            case Protocol::ANS_NAK:
                connection->write(Protocol::ANS_NAK);
                connection->write(p.numericValue);
                // connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
                // TODO: Write error message!
                break;
        }

    }

    // Is a command
    if (command <= Protocol::COM_END) {
        connection->write(Protocol::COM_END);
    } else {
        connection->write(Protocol::ANS_END);
    }

}

/*
 * Send an integer to the server as four bytes.
 */
void MessageHandler::write_number(const std::shared_ptr<Connection> &connection, int value) {
    connection->write((value >> 24) & 0xFF);
    connection->write((value >> 16) & 0xFF);
    connection->write((value >> 8)	 & 0xFF);
    connection->write(value & 0xFF);
}

void MessageHandler::write_string(const shared_ptr<Connection>& conn, string s) {
    write_number(conn, static_cast<int>(s.length()));

    for (char c : s) {
        conn->write(c);
    }
}

