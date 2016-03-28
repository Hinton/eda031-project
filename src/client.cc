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

bool is_number(const string& s)  {
	return all_of(s.cbegin(), s.cend() , [] (char c)  { return isdigit(c); });
}

int find_group_nbr(shared_ptr<RemoteDatabase> db, const string& id) {
	int group_nbr = -1;
	if (is_number(id)) {
		group_nbr = stoi(id);
	} else {
		auto groups = db->get_newsgroups();
		auto res = find_if(groups.begin(), groups.end(), [id] (shared_ptr<INewsgroup> p) { return p->get_title() == id; });
		if (res != groups.end()) {
			group_nbr = (*res)->get_id();
		}
	}
	return group_nbr;
}

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

	shared_ptr<RemoteDatabase> db = shared_ptr<RemoteDatabase> (new RemoteDatabase(shared_ptr<ServerCommunication>(&server)));

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
			for (auto &newsgroup : db->get_newsgroups()) {
				cout << newsgroup->get_id() <<  ". " << newsgroup->get_title() << endl;
			}
		} else if (cmd == "list" && args.size() == 1) { // list articles
			current_group = find_group_nbr(db, args[0]);
			if (current_group != -1) {
				for (auto &article : db->get_articles(current_group)) {
					cout << article->get_id() <<  ". " << article->get_title() << endl;
				}
			} else {
				cmd_err("No such newsgroup");
			}
			is_in_group = true;
		} else if (cmd == "create" && args.size() == 1) { // create newsgroup
			try {
				auto res = db->add_newsgroup(0, args[0]);
				cout << "Group successfully created" << endl;
				current_group = res->get_id();
				is_in_group = true;
			} catch (exception& e) {
				cmd_err("Failed, name already exists");
			}
		} else if (cmd == "create" && args.size() == 3 && is_in_group) { // create newsarticle in current group

			auto res = db->add_article(current_group, 0, args[0], args[1], args[2]);
			cout << "Article successfully created" << endl;
		} else if (cmd == "delete_art" && args.size() == 1 && is_in_group) { // delete article in group
			int art_nbr = 0;
			bool success = false;
			try {
				art_nbr = stoi(args[0]);
				success = db->remove_article(current_group, art_nbr);
			} catch (invalid_argument& e) {
				cmd_err("Article must be specified with a number");
				cmd_err(e.what());
				/*
			} catch (group_not_found& e) {
				cmd_err("No such newsgroup");
			} catch (article_not_found& e) {
				cmd_err("No such article");
				*/
			}
			if(success) {
				cout << "Article successfully removed" << endl;
			} else {
				cmd_err("Article could not be removed");
			}
		} else if(cmd == "delete_grp" && args.size() == 1) { // delete newsgroup
			int group_nbr = find_group_nbr(db, args[0]);
			bool success = db->remove_newsgroup(group_nbr);
			if (success) {
				cout << "Group successfully deleted" << endl;
			} else {
				cout << "Failed, no such group" << endl;
			}
		} else if (cmd == "read" && args.size() == 1 && is_in_group) { // read article in latest_group
			int art_nbr = 0;
			try {
				art_nbr = stoi(args[0]);
				auto art = db->get_article(current_group, art_nbr);
				cout << art->get_title() << "\t From: " << art->get_author() << endl;
					cout << art->get_text() << endl;
			} catch (exception& e) {
				cmd_err("Not an article number");
				cmd_err(e.what());
				/*
			} catch (group_not_found& e) {
				cmd_err("No such newsgroup");
			} catch (article_not_found& e) {
				cmd_err("No such article");
				*/
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
