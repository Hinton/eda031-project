#include "connection.h"
#include "connectionclosedexception.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 1234

using namespace std;


/*
 * Send an integer to the server as four bytes. Copied from myclient.cc
 */
void writeNumber(const Connection& conn, int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

/*
 * Read a string from the server. Copied from myclient.cc
 */
string readString(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
}

int main(int argc, char* argv[]) {
	const char* def_host = "localhost";
	int def_port = 1234;
	const char* host;
	int port;
	if (argc == 3) {
		host = argv[1];
		try {
			port = stoi(argv[2]);
		} catch (exception& e) {
			cerr << "Wrong port number" << e.what() << endl;
			return 1;
		}
	} else if (argc == 1) {
		host = def_host;
		port = def_port;
	} else {
		cerr << "Wrong number of arguments. Usage:"<< endl;
		cerr << "client (uses host=localhost and port=1234)" << endl;
		cerr << "client <host> <port>" << endl;
		return 1;
	}
	cout << "Connecting to: " << host << "@" << port << endl;
	Connection con(host, port);
	if (!con.isConnected()) {
		cerr << "Connection failed, exiting" << endl;
		return 1;
	}
	cout << "Connection succeeded" << endl;
	cout << "news>";
	string line;
	while(getline(cin, line)) {
		istringstream is(line);
		string cmd;
		vector<string> args;
		getline(is, cmd, ' '); // First word is command e.g. list
		string arg;
		while (getline(is, arg, ' ')) {
				args.push_back(arg);
		}

		cout << "news>";
	}
}
