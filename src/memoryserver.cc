#include <iostream>
#include "memoryserver.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "message.h"
#include "inmemorydatabase.h"

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

MemoryServer::MemoryServer(int port, IDatabase *database) : server(port), database(database), parser() {

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
				database->create_newsgroup(name);

				Message response(Protocol::ANS_CREATE_NG);

				MessageParam ack;
				ack.requestType = Protocol::ANS_ACK;
				response.add_param(ack);

				// TODO: Validate if the nwesgroup was created?
				/*
				MessageParam p1;
				p1.requestType = Protocol::ANS_NAK;
				p1.numericValue = Protocol::ERR_NG_ALREADY_EXISTS;
				params.push_back(p1);
				 */

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_DELETE_NG) {
				auto p = message.getParameters();
				int id = p[0].numericValue;

				bool success = database->delete_newsgroup(id);

				Message response(Protocol::ANS_DELETE_NG);

				if (success) {
					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);
				} else {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					response.add_param(nak);
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_LIST_ART) {
				auto p = message.getParameters();
				int id = p[0].numericValue;

				auto articles = database->list_articles(id);

				Message response(Protocol::ANS_LIST_ART);

				MessageParam ack;
				ack.requestType = Protocol::ANS_ACK;
				response.add_param(ack);

				response.add_number_param(articles.size());

				for (auto it = articles.begin(); it != articles.end(); it++) {
					response.add_number_param((*it)->get_id());
					response.add_string_param((*it)->get_title());
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_CREATE_ART) {
				auto p = message.getParameters();
				int id = p[0].numericValue;

				string title = p[1].textValue;
				string author = p[2].textValue;
				string text = p[3].textValue;

				database->create_article(id, title, author, text);

				Message response(Protocol::ANS_CREATE_ART);

				MessageParam ack;
				ack.requestType = Protocol::ANS_ACK;
				response.add_param(ack);

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_DELETE_ART) {

				auto p = message.getParameters();
				int nid = p[0].numericValue;
				int aid = p[0].numericValue;

				bool success = database->delete_article(nid, aid);

				Message response(Protocol::ANS_DELETE_ART);

				if (success) {
					MessageParam ack;
					ack.requestType = Protocol::ANS_ACK;
					response.add_param(ack);
				} else {
					MessageParam nak;
					nak.requestType = Protocol::ANS_NAK;
					nak.numericValue = Protocol::ERR_ART_DOES_NOT_EXIST;
					response.add_param(nak);
				}

				parser.send_message(conn, response);

			} else if (cmd == Protocol::COM_GET_ART) {

				auto p = message.getParameters();
				int nid = p[0].numericValue;
				int aid = p[0].numericValue;

				auto article = database->get_article(nid, aid);

				Message response(Protocol::ANS_GET_ART);

				MessageParam ack;
				ack.requestType = Protocol::ANS_ACK;
				response.add_param(ack);

				response.add_string_param(article->get_title());
				response.add_string_param(article->get_author());
				response.add_string_param(article->get_text());

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

	InMemoryDatabase database;

	// Setup the server
	MemoryServer memoryServer(1337, &database);
	memoryServer.run();

}
