#include "messagehandler.h"
#include <vector>

using namespace std;

MessageHandler::MessageHandler() {

}

Message MessageHandler::parseNext(const std::shared_ptr<Connection> &connection) {

    // Get the command;
    int command = connection->read();

    std::cout << command << endl;

    vector<MessageParameter> parameters;

    //int type = connection->read();
    //cout << type << endl;

    // Loop until command is done.
    unsigned char type = connection->read();
    while (type != Protocol::COM_END) {
        MessageParameter param;
        param.commandType = type;

        if (type == Protocol::PAR_NUM) {
            param.numericValue = read_number(connection);
        } else if (type == Protocol::PAR_STRING) {
            param.textValue = read_string(connection);
        } else {
            cerr << "UNKNOWN PARAMETER TYPE " << param.commandType << endl;
        }

        cout << "PARAM: " << endl;
        cout << param.commandType << " " << param.numericValue << " " << param.textValue << endl;

        parameters.push_back(param);

        type = connection->read();
    }

    //parameters.push_back(MessageParameter::)



    Message message;

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

std::string MessageHandler::read_string(const std::shared_ptr<Connection> &connection) {
    // How long is the string?
    int length = read_number(connection);

    string s;
    for (int i = 0; i != length; ++i) {
        s += connection->read();
    }

    return s;
}


