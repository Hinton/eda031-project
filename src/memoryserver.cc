#include <iostream>
#include "memoryserver.h"
#include "connection.h"
#include "connectionclosedexception.h"

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

            switch (message.getCommand()) {
                case Protocol::COM_LIST_NG:
                    cout << "LIST NEWGROOPS";
                    break;
                default:
                    cerr << "UNKNOWN COMMAND";
                    break;
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