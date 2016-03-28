#include "connection.h"
#include "connectionclosedexception.h"
#include "servercommunication.h"
#include "remotedatabase.h"
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"

#include <iterator> // for istream_iterator & back_inserter
#include <string>
#include <memory> 
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

void cmd_err(const string& err_msg) {
	cerr << err_msg << endl;
}

void print_help(ostream &o) {
	o << "----Newsgroup application----" << endl;
	o << "The application accepts a command followed by zero or more parameters." << endl;
	o << "Parameters that contain spaces needs to be enclosed in quotation marks, e.g. \"text with space\"." << endl;
	o << "[ x | y ] means either x or y." << endl;
	o << "Commands : Explanations" << endl;
	o << "list : Lists the available newsgroups." << endl;
	o << "list [Newsgroup name | Newsgroup number] : Lists the articles in the specified newsgroup." << endl;
	o << "create [Newsgroup name] : Creates a newsgroup with the specified name." << endl;
	o << "create [Article name] [Article author] [Article text] : Creates an article ine the previously listed newsgroup" << endl;
	o << "delete_grp [Newsgroup name | Newsgroup number] : Deletes the specified newsgroup." <<  endl;
	o << "delete_art [Article number] : Deletes the specified article from the previously listed newsgroup." << endl;
	o << "read [Article number] : Views the specified article from the previously listed newsgroup." << endl;
}

/**
 * Utility for main method. Returns the words separated by space in the string line.
 * Text enclosed in citation mark (") is regarded as one word.
 * Returns an empty vector if line was malformed
 */
vector<string> get_words(const string& line) {
	istringstream is(line);
	string part;	
	vector<string> v;
	while (getline(is, part, '"')) { // To " or EOF
		istringstream words(part);
		string word;
		while(words >> word) {
			v.push_back(word);
		}
		if(getline(is,part, '"')) {
			v.push_back(part);
		}
	}
 	return v;
}

int main(int argc, char* argv[]) {
	const char* def_host = "localhost";
	int def_port = 1337;
	const char* host;
	int port;
	if (argc == 3) {
		host = argv[1];
		try {
			port = stoi(argv[2]);
		} catch (exception& e) {
			cerr << "Wrong port number" << endl;
			cout << e.what() << endl;
			return 1;
		}
	} else if (argc == 1) {
		host = def_host;
		port = def_port;
	} else {
		cerr << "Wrong number of arguments. Usage:"<< endl;
		cerr << "client <host> <port>" << endl;
		cerr << "client (uses host=localhost and port=1234)" << endl;
		return 1;
	}
	cout << "Connecting to: " << host << "@" << port << endl;

	shared_ptr<Connection> con(new Connection(host, port));
	if (!con->isConnected()) {
		cerr << "Connection failed, exiting" << endl;
		return 1;
	}
	cout << "Connection succeeded" << endl;
	ServerCommunication server(con);
	cout << "news>";

	RemoteDatabase *db = new RemoteDatabase(shared_ptr<ServerCommunication>(&server));

	string line;
	int current_group = 0;
	bool is_in_group = false; // Whether or not current_group can be used
	while(getline(cin, line)) {
		vector<string> words = get_words(line);
		if (words.size() == 0) {
			cmd_err("Unrecognized command. See help section");
			cout << "news>";
			continue;
		}
		string cmd = words[0];
		vector<string> args(words.begin()+1, words.end());
		if (cmd == "list" && args.size() == 0) { // list newsgroups
			for (auto &newsgroup : db->list_newsgroups()) {
				cout << "Title: " << newsgroup->get_title() << endl;
			}
			/*vector<pair<int, string>> result = server.list_newsgroups();
			is_in_group = false;
			cout << "Available newsgroups:" << endl;
			for (auto it = result.begin(); it != result.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}	*/
		} else if (cmd == "list" && args.size() == 1) { // list articles
			pair<int, vector<pair<int, string>>> res = server.list_articles(args[0]);
			current_group = res.first;
			auto result = res.second;
			is_in_group = true;
			for (auto it = result.begin(); it != result.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}	
		} else if (cmd == "create" && args.size() == 1) { // create newsgroup
			pair<bool, int> res = server.create_newsgroup(args[0]);
			if (res.first) {
				cout << "Group successfully created" << endl;
				current_group = res.second;
				is_in_group = true;
			} else {
				cout << "Failed, name already exists" << endl;
			}
		} else if (cmd == "create" && args.size() == 3 && is_in_group) { // create newsarticle in current group
			bool success = server.create_article(current_group, args[0], args[1], args[2]);
			if (success) {
				cout << "Article successfully created" << endl;
			} else {
				cout << "Newsgroup does not exists" << endl;
			}
		} else if (cmd == "delete_art" && args.size() == 1 && is_in_group) { // delete article in group
			int art_nbr = 0;
			try {
				art_nbr = stoi(args[0]);
				string result = server.delete_article(current_group, art_nbr);
				cout << result << endl;
			} catch (exception& e) {
				cerr << "Not an article number" << endl;
				cerr << e.what() << endl;
			}
		} else if(cmd == "delete_grp" && args.size() == 1) { // delete newsgroup
			bool success = server.delete_newsgroup(args[0]);
			if (success) {
				cout << "Group successfully deleted" << endl;
			} else {
				cout << "Failed, no such group" << endl;
			}
		} else if (cmd == "read" && args.size() == 1 && is_in_group) { // read article in latest_group
			int art_nbr = 0;
			try {
				art_nbr = stoi(args[0]);
				pair<vector<string>, string> result = server.get_article(current_group, art_nbr);
				vector<string> article = result.first;
				string err_msg = result.second;
				if (article.size() != 0) {
					cout << article[0] << "\t From: " << article[1] << endl;
					cout << article[2] << endl;
				} else {
					cmd_err(err_msg);
				}
			} catch (exception& e) {
				cmd_err("Not an article number");
				cmd_err(e.what());
			}
		} else if (cmd == "help") {
			print_help(cout);
		} else if (cmd == "exit") {
			return 0;
		} else {
			cmd_err("Unrecognized command. See help section");
		}
		cout << "news>";
	}
}
