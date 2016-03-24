#include "messagehandler.h"

using namespace std;

MessageHandler::MessageHandler() {

}

// TODO: Throw errors on parsing issues?
Response MessageHandler::parse_next(const std::shared_ptr<Connection> &connection) {

    // Get the command;
    int command = connection->read();

    std::cout << "COMMAND: " << command << endl;

    vector<ResponseParam> parameters;

    // Loop until command is done.
    int type = connection->read();
    cout << type << endl;

    while (type != Protocol::COM_END) {
        ResponseParam param;
        param.requestType = type;

        if (type == Protocol::PAR_NUM) {
            param.numericValue = read_number(connection);
        } else if (type == Protocol::PAR_STRING) {
            param.textValue = read_string(connection);
        } else {
            cerr << "UNKNOWN PARAMETER TYPE " << param.requestType << endl;
        }

        // TODO: Handle ans? as they are only 1 char long they dont have any value?

        cout << "PARAM: " << endl;
        cout << param.requestType << " " << param.numericValue << " " << param.textValue << endl;

        parameters.push_back(param);

        type = connection->read();
        cout << type << endl;
    }

    return Response(command, parameters);
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


