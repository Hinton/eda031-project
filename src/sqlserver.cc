#include <iostream>
#include "newsserver.h"
#include "sqlitedatabase.h"

using namespace std;

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

	shared_ptr<IDatabase> database = shared_ptr<IDatabase>(new SqliteDatabase());

	// Setup the server
	MemoryServer memoryServer(port, database);
	memoryServer.run();

}
