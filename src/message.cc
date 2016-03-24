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

void Message::send(const shared_ptr<Connection> &connection) {

    // Send Protocol::<cmd>
    unsigned char cmd = static_cast<unsigned char>(command);
    connection->write(cmd);

    // Send parameters
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {

        MessageParam p = *it;

        switch (p.requestType) {

            case Protocol::PAR_NUM:
                connection->write(Protocol::PAR_NUM);
                write_number(connection, p.numericValue);
                break;
            case Protocol::PAR_STRING:
                connection->write(Protocol::PAR_STRING);
                break;
            case Protocol::ANS_ACK:
                connection->write(Protocol::ANS_ACK);
                break;
            case Protocol::ANS_NAK:
                connection->write(Protocol::ANS_NAK);

                // connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
                // TODO: Write error message!
                break;
        }

    }

}

/*
 * Send an integer to the server as four bytes.
 */
void Message::write_number(const std::shared_ptr<Connection> &connection, int value) {

    // Write the number
    connection->write((value >> 24) & 0xFF);
    connection->write((value >> 16) & 0xFF);
    connection->write((value >> 8)	 & 0xFF);
    connection->write(value & 0xFF);
}

void Message::write_string(const shared_ptr<Connection>& conn, string s) {
    write_number(conn, s.length());

    for (char c : s) {
        conn->write(c);
    }

    conn->write('$');
}
