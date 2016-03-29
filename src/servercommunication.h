#ifndef SERVERCOMMUNICATION_H
#define SERVERCOMMUNICATION_H

#include "connection.h"
#include "message.h"
#include "messagehandler.h"
#include "database_exceptions.h"

#include <vector> //vector
#include <memory> // shared_ptr


/**
 * Serves as an interface for the client to retrieve data from the server
 */
class ServerCommunication {
public:
	ServerCommunication(const std::shared_ptr<Connection>& con) : con(con), msg_handler() {}

	/**
	 * Lists the newsgroups on the server. Prints protocol errors to cerr and returns empty
	 * vector if fail.
	 */
	std::vector<std::pair<int, std::string>> list_newsgroups();

	/**
	 * Creates a new newsgroup on the server with the supplied name
	 * Throws obj_already_exists if there is a name conflict. 
	 */
	void create_newsgroup(const std::string& name);

	/**
	 * Deletes a newsgroup on the server with the supplied number
	 * Returns the status (success/fail)
	 */
	bool delete_newsgroup(const int group_nbr);

	/**
	 * List the articles in newsgroup with the supplied number
	 * Throws group_not_found if there is no group with that number.
	 * Returns vector<pair<article number, article title>>
	 */
	std::vector<std::pair<int, std::string>> list_articles(const int group_nbr);
	
	/**
 	 * Creates an article with the supplied content in the newsgroup identified by group_nbr
	 * Throws group_not_found if there is no group with that number
	 */
	void create_article(const int group_nbr, const std::string& title, const std::string& author, const std::string& text);

	/**
	 * Deletes the article in newsgroup group_nbr and number art_nbr.
	 * Returns status (success/fail)
	 * Throws group_not_found and article_not_found
	 */
	bool delete_article(const int group_nbr, const int art_nbr);

	/**
	 * Fetches an article with the number art_nbr,from newsgroup group_nbr.
	 * Returns the author, title and text as three elements in a vector
	 * Throws group_not_found() and article_not_found()
	 */
	std::vector<std::string> get_article(const int group_nbr, const int art_nbr);

private:
	const std::shared_ptr<Connection>& con;
	MessageHandler msg_handler;
	/**
	 * Utility function to convert either a newsgroup name or newsgroup number to a group nbr
	 * for sending to the server.
	 */

	void protocol_err(const std::string& sent, const std::string& expected, const int received) {
		std::cerr << "Protocol error: Sent " << sent << ", expected " << expected << ", received " << received << std::endl;
	}
};

#endif
