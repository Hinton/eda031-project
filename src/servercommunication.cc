#include "servercommunication.h"
#include "protocol.h"

#include <cctype> // isdigit
#include <algorithm> // find_if


using namespace std;



/**
 * Utility function, couldn't find one in standard lib
 */
bool ServerCommunication::is_number(const string& s) {
	return all_of(s.cbegin(), s.cend(), [](char c) { return isdigit(c); });
}

int ServerCommunication::find_group_nbr(const string& id) {
	int group_nbr = 0;
	if (is_number(id)) {
		group_nbr = stoi(id);
	} else {
		vector<pair<int, string>> groups = list_newsgroups();
		auto res = find_if(groups.begin(), groups.end(), [id] (pair<int, string>& p) { return p.second == id; });
		if (res != groups.end()) {
			group_nbr = (*res).first;
		}
	}
	return group_nbr;
}
// TODO throw exceptions instead of calling protocol_err?

vector<pair<int, string>> ServerCommunication::list_newsgroups() {
	Message msg(Protocol::COM_LIST_NG, {});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);
	vector<pair<int, string>> ret;
	if (reply.getType() != Protocol::ANS_LIST_NG) {
		vector<MessageParam> params = reply.getParameters();
		for (int i = 1; i != params[0].numericValue; i+=2) {
			ret.push_back(make_pair(params[i].numericValue, params[i+1].textValue));
		}
	} else {
		protocol_err("COM_LIST_NG", "ANS_LIST_NG", reply.getType());
	}
	return ret;
}

pair<bool, int> ServerCommunication::create_newsgroup(const string& name) {
	int group_nbr = 0;
	bool success = false;

	Message msg(Protocol::COM_CREATE_NG, {MessageParam(Protocol::PAR_STRING, name.size(), name)});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	if (reply.getType() == Protocol::ANS_CREATE_NG) {
		vector<MessageParam> reply_params = reply.getParameters();
		success = reply_params[0].numericValue == Protocol::ANS_ACK;
		if (success) {
			group_nbr = find_group_nbr(name);
		}
	} else {
		protocol_err("COM_CREATE_NG", "ANS_CREATE_NG", reply.getType());
	}
	cout << "createNewsGroup called with: " << name << endl;
	return make_pair(success, group_nbr);
}

bool ServerCommunication::delete_newsgroup(const string& id) {
	bool success = false;
	int group_nbr = find_group_nbr(id);

	Message msg(Protocol::COM_DELETE_NG, {MessageParam(Protocol::PAR_NUM, group_nbr, "")});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	if (reply.getType() == Protocol::ANS_DELETE_NG) {
		vector<MessageParam> reply_params = reply.getParameters();
		success = reply_params[0].numericValue == Protocol::ANS_ACK;
	} else {
		protocol_err("COM_DELETE_NG", "ANS_DELETE_NG", reply.getType());
	}
	cout << "deleteNewsGroup called" << "nbr: " << group_nbr << endl;
	return success;
}

pair<int, vector<pair<int, string>>> ServerCommunication::list_articles(const string& id) {
	int group_nbr = find_group_nbr(id);

	Message msg(Protocol::COM_LIST_ART, {MessageParam(Protocol::PAR_NUM, group_nbr, "")});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);
	// TODO Finish this
	vector<pair<int, string>> ret;
	if (reply.getType() != Protocol::ANS_LIST_ART) {
	} else {
		protocol_err("COM_LIST_ART", "ANS_LIST_ART", reply.getType());
	}
	cout << "list_articles called with " << id  << endl;
	return make_pair(group_nbr, ret);
}

string ServerCommunication::create_article(const int group_nbr, const string& title, const string& author, const string& text) {
	// TODO Finish this
	con->isConnected(); // To remove unused warnings
	// send create art msg
	// recieve reply
	// return result
	cout << "create_article called with, nbr: " << group_nbr << ", title: " << title << ", author: " << author << ", text: " << text << endl;
	return "create_articleReply";
}

string ServerCommunication::delete_article(const int group_nbr, const string& id) {
	// can only delete an article when called with a article number NOT with article name
	// TODO Fix this?
	// TODO Finish this
	con->isConnected(); // To remove unused warnings
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

vector<string> ServerCommunication::get_article(const int group_nbr, const string& title) {
	// TODO Finish this
	con->isConnected(); // To remove unused warnings
	cout << "get_article called wth: " << group_nbr << " " << title << endl;
	return { "<title>", "<author>", "Lorem ipsum..." } ;
}
