#include <iostream>
#include "memoryserver.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "message.h"

using namespace std;

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection> &conn, const string &s) {
    for (char c : s) {
        conn->write(c);
    }
    conn->write('$');
}

MemoryServer::MemoryServer(int port) : server(port), parser() {

    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(1);
    }

}

void MemoryServer::run() {
    while (true) {
        auto conn = server.waitForActivity();

        cout << "yey" << endl;

        // Setup the connection.
        if (conn == nullptr) {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }

        try {

            Message message = parser.parse_next(conn);
            cout << "123";

            int cmd = message.getType();
            if (cmd == Protocol::COM_LIST_NG) {

                vector<MessageParam> params;

                MessageParam p1;
                p1.requestType = Protocol::PAR_NUM;
                p1.numericValue = 0;
                params.push_back(p1);

                Message response(Protocol::ANS_LIST_NG, params);
                response.send(conn);

            } else if (cmd == Protocol::COM_CREATE_NG) {

                vector<MessageParam> params;

                MessageParam p1;
                p1.requestType = Protocol::ANS_NAK;
                p1.numericValue = Protocol::ERR_NG_ALREADY_EXISTS;
                params.push_back(p1);

                Message response(Protocol::ANS_CREATE_NG, params);
                response.send(conn);

            } else {
                cerr << "UNKNOWN COMMAND";
                throw ConnectionClosedException();
            }



            // FIXME: Remove! added due to console not showing for some reason
            break;

        } catch (ConnectionClosedException &) {
            server.deregisterConnection(conn);
            cout << "Client closed connection" << endl;
        }

    }

}

int main(int argc, char *argv[]) {

    // Try and find a port, fallback to 1337.
    int port = 1337;
    if (argc > 1) {
        try {
            port = stoi(argv[1]);
        } catch (exception &e) {
            cerr << "Wrong port number. " << e.what() << endl;
            exit(1);
        }
    }

    cout << "Starting server on port: " << port << endl;

    // Setup the server
    MemoryServer memoryServer(1337);
    memoryServer.run();

}