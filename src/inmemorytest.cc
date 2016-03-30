#include <iostream>
#include "database_exceptions.h"
#include "inmemorydatabase.h"
#include "inmemorynewsgroup.h"
#include "inmemoryarticle.h"

using namespace std;

void printNewsgroup(shared_ptr<InMemoryNewsgroup> newsgroup);

void printArticle(shared_ptr<InMemoryArticle> article);

void printAll(InMemoryDatabase &db);

int main(int argc, char *argv[]) {
	// Setup
	InMemoryDatabase db;
	auto newsgroup1 = dynamic_pointer_cast<InMemoryNewsgroup>(db.create_newsgroup("Newsgroup 1 title"));
	auto article1 = dynamic_pointer_cast<InMemoryArticle>(
			db.create_article(newsgroup1->get_id(), "Awesome title 1", "Samme", "nope"));
	auto article2 = dynamic_pointer_cast<InMemoryArticle>(
			db.create_article(newsgroup1->get_id(), "Bad title 1", "Samme", "yep"));

	auto newsgroup2 = dynamic_pointer_cast<InMemoryNewsgroup>(db.create_newsgroup("Newsgroup 2 title"));
	auto article3 = dynamic_pointer_cast<InMemoryArticle>(
			db.create_article(newsgroup2->get_id(), "Awesome title 2", "Samme", "nope"));
	auto article4 = dynamic_pointer_cast<InMemoryArticle>(
			db.create_article(newsgroup2->get_id(), "Bad title 2", "Samme", "yep"));

	/*
	cout << "Normal printout:" << endl;
	printNewsgroup(newsgroup1);
	printArticle(article1);
	printArticle(article2);

	printNewsgroup(newsgroup2);
	printArticle(article3);
	printArticle(article4);
	 */

	/*
	cout << "Pre-deletion printout:" << endl;
	printAll(db);
	*/

	// Newsgroup get test: Existing newsgroup
	try {
		auto newsgroup = db.get_newsgroup(newsgroup1->get_id());
		if (newsgroup == nullptr)
			throw runtime_error("Returned newsgroup was null.");
		cout << "Newsgroup get test: Existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup get test: Existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Newsgroup get test: Non-existing newsgroup
	try {
		auto newsgroup = db.get_newsgroup(1337);
		cout << "Newsgroup get test: Non-existing newsgroup FAILED!" << endl;
	}
	catch (group_not_found &) {
		cout << "Newsgroup get test: Non-existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup get test: Non-existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Newsgroup list test
	try {
		auto ng_vec = db.list_newsgroups();
		if (ng_vec.size() != 2)
			throw runtime_error("Wrong size of newsgroup vector.");
		for (auto it = ng_vec.begin(); it != ng_vec.end(); ++it) {
			if (*it == nullptr)
				throw runtime_error("Newsgroup pointer in vector was null.");
		}
		cout << "Newsgroup list test OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup list test FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Newsgroup creation test: Existing newsgroup
	try {
		db.create_newsgroup(newsgroup2->get_title());
		cout << "Newsgroup creation test: Existing newsgroup FAILED!" << endl;
	}
	catch (group_already_exists) {
		cout << "Newsgroup creation test: Existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup creation test: Existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Newsgroup deletion test: Non-existing newsgroup
	try {
		db.delete_newsgroup(1337);
		cout << "Newsgroup deletion test: Non-existing newsgroup FAILED!" << endl;
	}
	catch (group_not_found &) {
		cout << "Newsgroup deletion test: Non-existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup deletion test: Non-existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Newsgroup deletion test: Existing newsgroup
	try {
		db.delete_newsgroup(newsgroup2->get_id());
		if (db.list_newsgroups().size() != 1)
			throw runtime_error("Newsgroup was not properly deleted.");
		cout << "Newsgroup deletion test: Existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup deletion test: Existing newsgroup FAILED!" << endl;
		cout << e.what();
	}

	// Newsgroup get test: Non-existing newsgroup
	try {
		auto newsgroup = db.get_newsgroup(1337);
		cout << "Newsgroup get test: Non-existing newsgroup FAILED!" << endl;
	}
	catch (group_not_found &) {
		cout << "Newsgroup get test: Non-existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Newsgroup get test: Non-existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Article get test: Existing newsgroup
	try {
		auto article = db.get_article(newsgroup1->get_id(), article1->get_id());
		if (article == nullptr)
			throw runtime_error("Returned article was null.");
		cout << "Article get test: Existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Article get test: Existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Article get test: Non-existing newsgroup
	try {
		db.get_article(1337, article1->get_id());
		cout << "Article get test: Non-existing newsgroup FAILED!" << endl;
	}
	catch (group_not_found &) {
		cout << "Article get test: Non-existing newsgroup OK!" << endl;
	}
	catch (exception &e) {
		cout << "Article get test: Non-existing newsgroup FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Article get test: Non-existing article
	try {
		db.get_article(newsgroup1->get_id(), 1337);
		cout << "Article get test: Non-existing article FAILED!" << endl;
	}
	catch (article_not_found &) {
		cout << "Article get test: Non-existing article OK!" << endl;
	}
	catch (exception &e) {
		cout << "Article get test: Non-existing article FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Article list test
	try {
		auto art_vec = db.list_articles(newsgroup1->get_id());
		if (art_vec.size() != 2)
			throw runtime_error("Wrong size of newsgroup vector.");
		for (auto it = art_vec.begin(); it != art_vec.end(); ++it) {
			if (*it == nullptr)
				throw runtime_error("Article pointer in vector was null.");
		}
		cout << "Article list test OK!" << endl;
	}
	catch (exception &e) {
		cout << "Article list test FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Article deletion test: Non-existing article
	try {
		db.delete_article(newsgroup1->get_id(), 1337);
		cout << "Article deletion test: Non-existing article FAILED!" << endl;
	}
	catch (article_not_found &e) {
		cout << "Article deletion test: Non-existing article OK!" << endl;
	}
	catch (exception &e) {
		cout << "Article deletion test: Non-existing article FAILED!" << endl;
		cout << e.what() << endl;
	}

	// Article deletion test: Existing article
	try {
		db.delete_article(newsgroup1->get_id(), article1->get_id());
		if (db.list_articles(newsgroup1->get_id()).size() != 1)
			throw runtime_error("Article was not properly deleted.");
		cout << "Article deletion test: Existing article OK!" << endl;
	}
	catch (exception &e) {
		cout << "Article deletion test: Existing article FAILED!" << endl;
		cout << e.what() << endl;
	}

	/*
	cout << endl << "Post-deletion printout:" << endl;
	printAll(db);
	*/
}

void printNewsgroup(shared_ptr<InMemoryNewsgroup> newsgroup) {
	cout << "### Newsgroup ###" << endl;
	cout << "ID: " << newsgroup->get_id() << endl;
	cout << "Title: " << newsgroup->get_title() << endl;
	cout << endl;
}

void printArticle(shared_ptr<InMemoryArticle> article) {
	cout << "--- Article ---" << endl;
	cout << "ID: " << article->get_id() << endl;
	cout << "Title: " << article->get_title() << endl;
	cout << "Author: " << article->get_author() << endl;
	cout << "Text: " << article->get_text() << endl;
	cout << endl;
}

void printAll(InMemoryDatabase &db) {
	auto ng_vec = db.list_newsgroups();
	for (auto ng_it = ng_vec.begin(); ng_it != ng_vec.end(); ++ng_it) {
		auto newsgroup = dynamic_pointer_cast<InMemoryNewsgroup>(*ng_it);
		printNewsgroup(newsgroup);
		auto art_vec = db.list_articles(newsgroup->get_id());
		for (auto art_it = art_vec.begin(); art_it != art_vec.end(); ++art_it) {
			printArticle(dynamic_pointer_cast<InMemoryArticle>(*art_it));
		}
		cout << string('-', 20) << endl;
	}
}
