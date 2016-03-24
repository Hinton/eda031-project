#include "connection.h"
#include "connectionclosedexception.h"
#include "message.h"
#include "messagehandler.h"

#include <cctype> // for isdigit
#include <iterator> // for istream_iterator & back_inserter
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

/**
 * Utility function, could find one in standard lib
 */
bool is_number(const string& s) {
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
vector<pair<int, string>> list_newsgroups(const Connection& con) {	
	// TODO send msg, receive reply
	con.isConnected(); // To remove unused warnings
	cout << "listNewsGroup called" << endl;
	return {{0, "group0"}, {1, "group1"}};
}

/**
 * Utility function to convert either a newsgroup name or newsgroup number to a group nbr
 * for sending to the server.
 */
int find_group_nbr(const Connection& con, const string& id) {
	int group_nbr = -1;
	if (is_number(id)) {
		group_nbr = stoi(id);
	} else {
		vector<pair<int, string>> groups = list_newsgroups(con);
		auto res = find_if(groups.begin(), groups.end(), [id] (pair<int, string>& p) { return p.second == id; });
		if (res != groups.end()) {
			group_nbr = (*res).first;
		}
	}
	return group_nbr;
}

/**
 * Creates a new newsgroup on the server with the supplied name
 * Returns the number of the created group (-1 if fail) 
 * and status as string (e.g. success/failure/already exists)
 */
pair<int, string> create_newsgroup(const Connection& con, const string& name) {
	int group_nbr = -1;
	bool success = false;
	// send create_msg
	// receive reply
	if (success) {
		group_nbr = find_group_nbr(con, name);
	}
	cout << "createNewsGroup called with: " << name << endl;
	return make_pair(group_nbr, "createNewsGroupReply");
}

/**
 * Deletes a newsgroup on the server with the supplied [name | number]
 * Returns the status (success/fail) as string
 */
string delete_newsgroup(const Connection& con, const string& id) {
	int group_nbr = find_group_nbr(con, id);
	cout << "deleteNewsGroup called" << "nbr: " << group_nbr << endl;
	return "deleteNewsGroupReply";
}

/**
 * List the articles in newsgroup with id.
 * Id can either be a group number or group name.
 * Returns pair<group number, pair<article number, article string>>
 */
pair<int, vector<pair<int, string>>> list_articles(const Connection& con, const string& id) {
	int group_nbr = find_group_nbr(con, id);
	// send msg
	// recive reply
	cout << "list_articles called with " << id  << endl;
	vector<pair<int, string>> tmp =  {{0, "art0"}, {1, "art1"}};
	return make_pair(group_nbr, tmp);
}

/**
 * Creates an article with the supplied content in the newsgroup identified by group_nbr
 * Returns status (success/fail) as a string
 */
string create_article(const Connection& con, const int group_nbr, const string& title, const string& author, const string& text) {
	con.isConnected(); // To remove unused warnings
	// send create art msg
	// recieve reply
	// return result
	cout << "create_article called with, nbr: " << group_nbr << ", title: " << title << ", author: " << author << ", text: " << text << endl;
	return "create_articleReply";
}

/**
 * Deletes the article in newsgroup group_nbr and title id.
 * Returns status as string
 */
string delete_article(const Connection& con, const int group_nbr, const string& id) {
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
	cout << "delete_article called with " << group_nbr << " " <<  art_nbr << endl;
	return "deleterticleReply";
}
/**
 * Fetches an article witht the title 'title',from newsgroup group_nbr.
 * Returns the author, title and text as three elements in a vector
 */
vector<string> get_article(const Connection& con, const int group_nbr, const string& title) {
	con.isConnected(); // To remove unused warnings
	cout << "get_article called wth: " << group_nbr << " " << title << endl;
	return { "<title>", "<author>", "Lorem ipsum..." } ;
}

void cmd_err(const string& err_msg) {
	cerr << err_msg << endl;
}

void print_help(ostream &o) {
	o << "----Newsgroup application----" << endl;
	o << "The application accepts a command followed by zero or more parameters." << endl;
	o << "Parameters that contain spaces needs to be enclosed in quotation marks, e.g. \"text with space\"." << endl;
	o << "[ x | y ] means either x or y." << endl;
	o << "Commands : Explanation" << endl;
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
		cerr << "client <host> <port>" << endl;
		cerr << "client (uses host=localhost and port=1234)" << endl;
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
		vector<string> words = get_words(line);
		if (words.size() == 0) {
			cerr << "Unrecognized command. See help section" << endl;
			cout << "news>";
			continue;
		}
		string cmd = words[0];
		vector<string> args(words.begin()+1, words.end());
		if (cmd == "list" && args.size() == 0) { // list newsgroups
			vector<pair<int, string>> result = list_newsgroups(con);
			is_in_group = false;
			for (auto it = result.begin(); it != result.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}	
		} else if (cmd == "list" && args.size() == 1) { // list articles
			pair<int, vector<pair<int, string>>> res = list_articles(con, args[0]);
			current_group = res.first;
			auto result = res.second;
			is_in_group = true;
			for (auto it = result.begin(); it != result.end(); ++it) {
				cout << it->first << ". " << it->second << endl;
			}	
		} else if (cmd == "create" && args.size() == 1) { // create newsgroup
			pair<int, string> res = create_newsgroup(con, args[0]);
			cout << res.second << endl;
			current_group = res.first;
			is_in_group = true;
		} else if (cmd == "create" && args.size() == 3 && is_in_group) { // create newsarticle in current group
			string result = create_article(con, current_group, args[0], args[1], args[2]);
			cout << result << endl;
		} else if (cmd == "delete_art" && args.size() == 1 && is_in_group) { // delete article in group
			string result = delete_article(con, current_group, args[0]);
			cout << result << endl;
		} else if(cmd == "delete_grp" && args.size() == 1) { // delete newsgroup
			string result = delete_newsgroup(con, args[0]);
			cout << result << endl;
		} else if (cmd == "read" && args.size() == 1 && is_in_group) { // read article in latest_group
			vector<string> result = get_article(con, current_group, args[0]);
			cout << result[0] << "\t From: " << result[1] << endl;
			cout << result[2] << endl;
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
