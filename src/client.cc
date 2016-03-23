#include "connection.h"
#include "connectionclosedexception.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 1234

using namespace std;

vector<pair<int, string>> listNewsgroups(const Connection& con) {
	cout << "listNewsGroup called" << endl;
	return {{0, "test"}, {1, "test1"}};
}

string createNewsgroup(const Connection& con, const string& title) {
	cout << "createNewsGroup called" << endl;
	return "createNewsGroupReply";
}

string deleteNewsgroup(const Connection& con, const int group_nbr) {
	cout << "deleteNewsGroup called" << endl;
	return "deleteNewsGroupReply";
}

vector<pair<int, string>> listArticles(const Connection& con, const int group_nbr) {
	cout << "listArticles called" << endl;
	return {{0, "art0"}, {1, "art1"}};
}

string createArticle(const Connection& con, const int group_nbr, const string& title, const string& author, const string& text) {
	cout << "createArticle called" << endl;
	return "createArticleReply";
}
string deleteArticle(const Connection& con, const int group_nbr, const string& title) {
	cout << "deleteArticle called" << endl;
	return "deleterticleReply";
}

vector<string> getArticle(const Connection& con, const int group_nbr, const string& title) {
	cout << "getArticle called" << endl;
	return { "<title>", "<author>", "Lorem ipsum..." } ;
}

void cmd_err(const string& err_msg) {
	cerr << err_msg << endl;
}



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
	cout << "Still in testing, does not actually connect" << endl;

	Connection con(host, port);
	//if (!con.isConnected()) {
	//	cerr << "Connection failed, exiting" << endl;
	//	return 1;
	// }
	cout << "Connection succeeded" << endl;
	cout << "news>";
	string line;
	int latest_group = -1; // used to remembered which newsgroup was listed previously
	while(getline(cin, line)) {
		istringstream is(line);
		string cmd;
		vector<string> args;
		getline(is, cmd, ' '); // First word is command e.g. list
		string arg;
		while (getline(is, arg, ' ')) {
			args.push_back(arg);
		}
		if (cmd == "list" && args.size() == 0) { // List newsgroups
			vector<pair<int,string>> groups = listNewsgroups(con);
			for (auto it = groups.begin(); it != groups.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}
			latest_group = -1;
		} else if (cmd == "create" && args.size() == 1) { // create newsgroup
			// Possibly set latest_group here as well
			latest_group = -1;
			string result = createNewsgroup(con, args[0]);
			cout << result << endl;
		} else if (cmd == "delete" && args.size() == 1 && latest_group == -1) { // delete newsgroup
			// Lookup group_nbr from name (or convert if already number?)
			int group_nbr = 0;
			string result = deleteNewsgroup(con, group_nbr);
			cout << result << endl;
		} else if (cmd == "list" && args.size() == 1) { // list articles in newsgroup
			// we need to do a lookup on the newsgroup name here
			// group_nbr = func(args[0]) 
			int group_nbr = 0;
			vector<pair<int,string>> articles = listArticles(con, group_nbr);
			for (auto it = articles.begin(); it != articles.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}
			latest_group = group_nbr;
		} else if (cmd == "create" && args.size() == 3){ // create article in latest_group
			string result = createArticle(con, latest_group, args[0], args[1], args[2]);
			cout << result << endl;
		} else if (cmd == "delete" && args.size() == 1) { // delete article in latest_group
			string result = deleteArticle(con, latest_group, args[0]);
			cout << result << endl;
		} else if (cmd == "read" && args.size() == 1) { // read article in latest_group
			vector<string> result = getArticle(con, latest_group, args[0]);
			cout << result[0] << "\t From: " << result[1] << endl;
			cout << result[2] << endl;
		} else if (cmd == "exit") { // exit
			return 0;
		} else {
			cmd_err("No such command");
		}
		cout << "news>";
	}
}
