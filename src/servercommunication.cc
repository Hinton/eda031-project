#include "servercommunication.h"
#include "protocol.h"
#include "database_exceptions.h"

#include <cctype> // isdigit
#include <algorithm> // find_if


using namespace std;


// TODO throw exceptions instead of calling protocol_err?

vector<pair<int, string>> ServerCommunication::list_newsgroups() {
	Message msg(Protocol::COM_LIST_NG, {});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);
	vector<pair<int, string>> ret;
	if (reply.getType() == Protocol::ANS_LIST_NG) {
		vector<MessageParam> reply_params = reply.getParameters();
		for (size_t i = 1; i != reply_params.size(); i+=2) {
			int group_nbr = reply_params[i].numericValue;
			string title = reply_params[i+1].textValue;
			ret.push_back(make_pair(group_nbr, title));
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
		}
	} else {
		protocol_err("COM_CREATE_NG", "ANS_CREATE_NG", reply.getType());
	}
	return make_pair(success, group_nbr);
}

bool ServerCommunication::delete_newsgroup(const int group_nbr) {
	bool success = false;

	Message msg(Protocol::COM_DELETE_NG, {MessageParam(Protocol::PAR_NUM, group_nbr, "")});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	if (reply.getType() == Protocol::ANS_DELETE_NG) {
		vector<MessageParam> reply_params = reply.getParameters();
		success = reply_params[0].numericValue == Protocol::ANS_ACK;
	} else {
		protocol_err("COM_DELETE_NG", "ANS_DELETE_NG", reply.getType());
	}
	return success;
}

pair<int, vector<pair<int, string>>> ServerCommunication::list_articles(const int group_nbr) {

	Message msg(Protocol::COM_LIST_ART, {MessageParam(Protocol::PAR_NUM, group_nbr, "")});
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	vector<pair<int, string>> ret;
	if (reply.getType() == Protocol::ANS_LIST_ART) {
		vector<MessageParam> reply_params = reply.getParameters();
		if (reply_params[0].numericValue == Protocol::ANS_ACK) {
			for (int i = 1; i != reply_params[1].numericValue; i+=2) {
				ret.push_back(make_pair(reply_params[i].numericValue, reply_params[i+1].textValue));
			}
		} 
	} else {
		protocol_err("COM_LIST_ART", "ANS_LIST_ART", reply.getType());
	}
	return make_pair(group_nbr, ret);
}

bool ServerCommunication::create_article(const int group_nbr, const string& title, const string& author, const string& text) {
	bool success = false;
	vector<MessageParam> msg_params {
		MessageParam(Protocol::PAR_NUM, group_nbr, ""),
		MessageParam(Protocol::PAR_STRING, title.size(), title),
		MessageParam(Protocol::PAR_STRING, author.size(), author),
		MessageParam(Protocol::PAR_STRING, text.size(), text)};

	Message msg(Protocol::COM_CREATE_ART, msg_params);
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	vector<pair<int, string>> ret;
	if (reply.getType() == Protocol::ANS_CREATE_ART) {
		vector<MessageParam> reply_params = reply.getParameters();
		success = reply_params[0].numericValue == Protocol::ANS_ACK;
	} else {
		protocol_err("COM_CREATE_ART", "ANS_CREATE_ART", reply.getType());
	}
	return success;
}

string ServerCommunication::delete_article(const int group_nbr, const int art_nbr) {
	string result = "";

	vector<MessageParam> msg_params {
		MessageParam(Protocol::PAR_NUM, group_nbr, ""),
		MessageParam(Protocol::PAR_NUM, art_nbr, "")};

	Message msg(Protocol::COM_DELETE_ART, msg_params);
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	if (reply.getType() == Protocol::ANS_DELETE_ART) {
		vector<MessageParam> reply_params = reply.getParameters();
		if (reply_params[0].numericValue == Protocol::ANS_ACK) {
			result = "Success";
		} else if (reply_params[1].numericValue == Protocol::ERR_NG_DOES_NOT_EXIST) {
			throw group_not_found();
		} else {
			throw article_not_found();
		}
	} else {
		protocol_err("COM_DELETE_ART", "ANS_DELETE_ART", reply.getType());
	}
	return result;
}


pair<vector<string>, string> ServerCommunication::get_article(const int group_nbr, const int art_nbr) {
	vector<string> ret;
	string err_msg = "";


	vector<MessageParam> msg_params {
		MessageParam(Protocol::PAR_NUM, group_nbr, ""),
		MessageParam(Protocol::PAR_NUM, art_nbr, "")};

	Message msg(Protocol::COM_GET_ART, msg_params);
	msg_handler.send_message(con, msg);
	Message reply = msg_handler.parse_message(con);

	if (reply.getType() == Protocol::ANS_GET_ART) {
		vector<MessageParam> reply_params = reply.getParameters();
		if (reply_params[0].numericValue == Protocol::ANS_ACK) {
			ret.push_back(reply_params[1].textValue); // title
			ret.push_back(reply_params[2].textValue); // author
			ret.push_back(reply_params[3].textValue); // text
		} else if (reply_params[1].numericValue == Protocol::ERR_NG_DOES_NOT_EXIST) {
			err_msg = "Newsgoup does not exist";
		} else {
			err_msg = "Article does not exist";
		}
	} else {
		protocol_err("COM_GET_ART", "ANS_GET_ART", reply.getType());
	}
	return make_pair(ret, err_msg);
}
