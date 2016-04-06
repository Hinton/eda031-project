#include <iostream>
#include "memoryserver.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "message.h"
//#include "inmemorydatabase.h"
#include "sqlitedatabase.h"
#include "database_exceptions.h"

using namespace std;

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection> &conn, const string &s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
}

MemoryServer::MemoryServer(int port, shared_ptr<IDatabase> database) : server(port), database(database), parser() {

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

			Message message = parser.parse_message(conn);
			cout << "123";

			int cmd = message.getType();
			if (cmd == Protocol::COM_LIST_NG) {

				auto newsgroups = database->list_newsgroups();

				Message response(Protocol::ANS_LIST_NG);
				response.add_number_param(newsgroups.size());

				for (auto it = newsgroups.begin(); it != newsgroups.end(); it++) {
					response.add_number_param((*it)->get_id());
					response.add_string_param((*it)->get_title());
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_CREATE_NG) {

				auto p = message.getParameters();
				string name = p[0].textValue;

				Message response(Protocol::ANS_CREATE_NG);

				try {
					database->create_newsgroup(name);

					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);
				} catch (const group_already_exists &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_NG_ALREADY_EXISTS;
					response.add_param(nak);
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_DELETE_NG) {
				auto p = message.getParameters();
				int id = p[0].numericValue;

				Message response(Protocol::ANS_DELETE_NG);

				try {

					bool success = database->delete_newsgroup(id);

					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);

				} catch (const group_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_NG_DOES_NOT_EXIST;
					response.add_param(nak);
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_LIST_ART) {
				auto p = message.getParameters();
				int id = p[0].numericValue;

				Message response(Protocol::ANS_LIST_ART);

				try {
					auto articles = database->list_articles(id);

					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);

					response.add_number_param(articles.size());

					for (auto it = articles.begin(); it != articles.end(); it++) {
						response.add_number_param((*it)->get_id());
						response.add_string_param((*it)->get_title());
					}
				} catch (const group_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_NG_DOES_NOT_EXIST;
					response.add_param(nak);
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_CREATE_ART) {
				auto p = message.getParameters();
				int id = p[0].numericValue;

				string title = p[1].textValue;
				string author = p[2].textValue;
				string text = p[3].textValue;

				Message response(Protocol::ANS_CREATE_ART);

				try {
					database->create_article(id, title, author, text);

					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);

				} catch (const group_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_NG_DOES_NOT_EXIST;
					response.add_param(nak);
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_DELETE_ART) {

				auto p = message.getParameters();
				int nid = p[0].numericValue;
				int aid = p[1].numericValue;

				Message response(Protocol::ANS_DELETE_ART);

				try {
					bool success = database->delete_article(nid, aid);

					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);
				} catch (const group_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_NG_DOES_NOT_EXIST;
					response.add_param(nak);
				} catch (const article_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_ART_DOES_NOT_EXIST;
					response.add_param(nak);
				}
				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_GET_ART) {

				auto p = message.getParameters();
				int nid = p[0].numericValue;
				int aid = p[1].numericValue;

				Message response(Protocol::ANS_GET_ART);

				try {

					auto article = database->get_article(nid, aid);

					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);

					response.add_string_param(article->get_title());
					response.add_string_param(article->get_author());
					response.add_string_param(article->get_text());
				} catch (const group_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_NG_DOES_NOT_EXIST;
					response.add_param(nak);
				} catch (const article_not_found &e) {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_ART_DOES_NOT_EXIST;
					response.add_param(nak);
				}
				parser.send_message(conn, response);

			} else {
				cerr << "UNKNOWN COMMAND";
				throw ConnectionClosedException();
			}

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

	//shared_ptr<IDatabase> database = shared_ptr<IDatabase>(new InMemoryDatabase());
	shared_ptr<IDatabase> database = shared_ptr<IDatabase>(new SqliteDatabase());

	// Setup the server
	MemoryServer memoryServer(port, database);
	memoryServer.run();

}
