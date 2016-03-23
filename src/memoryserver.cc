#include <iostream>
#include "memoryserver.h"
#include "connection.h"
#include "connectionclosedexception.h"

using namespace std;

/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection> &conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

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

            Message message = parser.parseNext(conn);

            cout << "Got message" << endl;

            /*
            int nbr = readNumber(conn);
            string result;
            if (nbr > 0) {
                result = "positive";
            } else if (nbr == 0) {
                result = "zero";
            } else {
                result = "negative";
            }
            writeString(conn, result);
            */
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