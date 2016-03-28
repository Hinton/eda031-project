#ifndef SERVERCOMMUNICATION_H
#define SERVERCOMMUNICATION_H

#include "connection.h"
#include "message.h"
#include "messagehandler.h"

#include <vector> //vector
#include <memory> // shared_ptr


/**
 * Serves as an interface for the client to retrieve data from the server
 */
class ServerCommunication {
public:
	ServerCommunication(const std::shared_ptr<Connection>& con) : con(con), msg_handler() {}

	/**
	 * Lists the newsgroups on the server. Prints to cerr and returns empty
	 * vector if fail.
	 */
	std::vector<std::pair<int, std::string>> list_newsgroups();

	/**
	 * Creates a new newsgroup on the server with the supplied name
	 * Returns the status (success/fail) and the number of the created group 
	 */
	std::pair<bool, int> create_newsgroup(const std::string& name);

	/**
	 * Deletes a newsgroup on the server with the supplied [name | number]
	 * Returns the status (success/fail) as string
	 */
	bool delete_newsgroup(const int group_nbr);

	/**
	 * List the articles in newsgroup with id.
	 * Id can either be a group number or group name.
	 * Vector is empty and group_nbr is -1  if the group cant be found.
	 * Returns pair<group number, pair<article number, article string>>
	 */
	std::pair<int, std::vector<std::pair<int, std::string>>> list_articles(const int group_nbr);
	
	/**
 	 * Creates an article with the supplied content in the newsgroup identified by group_nbr
	 * Returns status (success/fail)
	 */
	bool create_article(const int group_nbr, const std::string& title, const std::string& author, const std::string& text);

	/**
	 * Deletes the article in newsgroup group_nbr and number art_nbr.
	 * Returns status as string
	 */
	std::string delete_article(const int group_nbr, const int art_nbr);

	/**
	 * Fetches an article with the number art_nbr,from newsgroup group_nbr.
	 * Returns the author, title and text as three elements in a vector
	 */
	std::pair<std::vector<std::string>, std::string> get_article(const int group_nbr, const int art_nbr);

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
