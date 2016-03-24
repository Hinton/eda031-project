#include "connection.h"
#include "connectionclosedexception.h"
#include "command.h"
#include "messagehandler.h"

#include <cctype> // for isdigit()
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 1234

using namespace std;

/**
 * Utility function, could find one in standard lib
 */
bool isNumber(const string& s) {
	for(auto it = s.begin(); it != s.end(); ++it) {
		if (!isdigit(*it)) {
			return false;
		}
	}
	return true;
}

/**
 * Lists the newsgroups on the server one the other side of Connection.
 */
vector<pair<int, string>> listNewsgroups(const Connection& con) {	
	// TODO send msg, receive reply
	con.isConnected(); // To remove unused warnings
	cout << "listNewsGroup called" << endl;
	return {{0, "group0"}, {1, "group1"}};
}

/**
 * Utility function to convert either a newsgroup name or newsgroup number to a group nbr
 * for sending to the server.
 */
int findGroupNbr(const Connection& con, const string& id) {
	int group_nbr = -1;
	if (isNumber(id)) {
		group_nbr = stoi(id);
	} else {
		vector<pair<int, string>> groups = listNewsgroups(con);
		auto res = find_if(groups.begin(), groups.end(), [id] (pair<int, string>& p) { return p.second == id; });
		group_nbr = (*res).first;
	}
	return group_nbr;
}

/**
 * Creates a new newsgroup on the server with the supplied name
 * Returns the number of the created group (-1 if fail) 
 * and status as string (e.g. success/failure/already exists)
 */
pair<int, string> createNewsgroup(const Connection& con, const string& name) {
	int group_nbr = -1;
	bool success = false;
	// send create_msg
	// receive reply
	if (success) {
		group_nbr = findGroupNbr(con, name);
	}
	cout << "createNewsGroup called with: " << name << endl;
	return make_pair(group_nbr, "createNewsGroupReply");
}

/**
 * Deletes a newsgroup on the server with the supplied [name | number]
 * Returns the status (success/fail) as string
 */
string deleteNewsgroup(const Connection& con, const string& id) {
	int group_nbr = findGroupNbr(con, id);
	cout << "deleteNewsGroup called" << "nbr: " << group_nbr << endl;
	return "deleteNewsGroupReply";
}

/**
 * List the articles in newsgroup with id.
 * Id can either be a group number or group name.
 * Returns pair<group number, pair<article number, article string>>
 */
pair<int, vector<pair<int, string>>> listArticles(const Connection& con, const string& id) {
	int group_nbr = findGroupNbr(con, id);
	// send msg
	// recive reply
	cout << "listArticles called with " << id  << endl;
	vector<pair<int, string>> tmp =  {{0, "art0"}, {1, "art1"}};
	return make_pair(group_nbr, tmp);
}

/**
 * Creates an article with the supplied content in the newsgroup identified by group_nbr
 * Returns status (success/fail) as a string
 */
string createArticle(const Connection& con, const int group_nbr, const string& title, const string& author, const string& text) {
	con.isConnected(); // To remove unused warnings
	// send create art msg
	// recieve reply
	// return result
	cout << "createArticle called with: " << group_nbr << " " << title << " " << author << " " << text << endl;
	return "createArticleReply";
}
string deleteArticle(const Connection& con, const int group_nbr, const string& id) {
	// can only delete an article when called with a article number NOT with article name
	// TODO Fix this?
	con.isConnected(); // To remove unused warnings
	int art_nbr = 0;
	try {
		art_nbr = stoi(id);
	} catch (exception& e) {
		return "Not an article number";
	}

	// send msg
	// Message reply = MessageHandler::parseNext(make_shared<Connection>(&con));
	cout << "deleteArticle called with " << group_nbr << " " <<  art_nbr << endl;
	return "deleterticleReply";
}

vector<string> getArticle(const Connection& con, const int group_nbr, const string& title) {
	con.isConnected(); // To remove unused warnings
	cout << "getArticle called wth: " << group_nbr << " " << title << endl;
	return { "<title>", "<author>", "Lorem ipsum..." } ;
}

void cmd_err(const string& err_msg) {
	cerr << err_msg << endl;
}

void printHelp(ostream &o) {
	o << "help called" << endl;
}

/**
 * Utility for main method. Returns the words separated by space in the string line.
 * Text enclosed in citation mark (") are regarded as one word.
 * Returns an empty vector if line was malformed
 */
vector<string> getWords(const string& line) {
	istringstream is(line);
	string word;	
	vector<string> v;
	while (getline(is, word, ' ')) {
		if (word[0] == '"') {
			string tmp(word.begin()+1, word.end());
			tmp += " ";
			if(getline(is, word, '"')) {
				v.push_back(tmp += word);
			} else { 
				return vector<string>();
			}
		} else {
			v.push_back(word);
		}
	}
	return v;
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
	// TODO Uncomment this when live
	//if (!con.isConnected()) {
	//	cerr << "Connection failed, exiting" << endl;
	//	return 1;
	// }
	cout << "Connection succeeded" << endl;
	cout << "news>";
	string line;
	int current_group = 0;
	bool is_in_group = false; // Whether or not current_group can be used
	while(getline(cin, line)) {
		vector<string> words = getWords(line);
		if (words.size() == 0) {
			cerr << "Unrecognized command. See help section";
			continue;
		}
		string cmd = words[0];
		vector<string> args(words.begin()+1, words.end());
		if (cmd == "list" && args.size() == 0) { // list newsgroups
			vector<pair<int, string>> result = listNewsgroups(con);
			is_in_group = false;
			for (auto it = result.begin(); it != result.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}	
		} else if (cmd == "list" && args.size() == 1) { // list articles
			pair<int, vector<pair<int, string>>> res = listArticles(con, args[0]);
			current_group = res.first;
			auto result = res.second;
			is_in_group = true;
			for (auto it = result.begin(); it != result.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}	
		} else if (cmd == "create" && args.size() == 1) { // create newsgroup
			pair<int, string> res = createNewsgroup(con, args[0]);
			cout << res.second << endl;
			current_group = res.first;
			is_in_group = true;
		} else if (cmd == "create" && args.size() == 3 && is_in_group) { // create newsarticle in current group
			string result = createArticle(con, current_group, args[0], args[1], args[2]);
			cout << result << endl;
		} else if (cmd == "delete" && args.size() == 1 && is_in_group) { // delete article in group
			string result = deleteArticle(con, current_group, args[0]);
			cout << result << endl;
		} else if(cmd == "delete" && args.size() == 1 && !is_in_group) { // delete newsgroup
			string result = deleteNewsgroup(con, args[0]);
				cout << result << endl;
		} else if (cmd == "read" && args.size() == 1 && is_in_group) { // read article in latest_group
			vector<string> result = getArticle(con, current_group, args[0]);
			cout << result[0] << "\t From: " << result[1] << endl;
			cout << result[2] << endl;
		} else if (cmd == "help") {
			printHelp(cout);
		} else if (cmd == "exit") {
			return 0;
		} else {
			cmd_err("No such command or invalid use of command. Write \"help\"");
		}
		cout << "news>";
	}
}
