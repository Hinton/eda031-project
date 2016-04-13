#include <iostream>
#include "database_exceptions.h"
#include "inmemorydatabase.h"
#include "inmemorynewsgroup.h"
#include "inmemoryarticle.h"
#include "sqlitedatabase.h"
#include "sqlitenewsgroup.h"
#include "sqlitearticle.h"

using namespace std;

void printNewsgroup(shared_ptr<INewsgroup> newsgroup);

void printArticle(shared_ptr<IArticle> article);

void printAll(shared_ptr<IDatabase> db);

int main(int argc, char *argv[]) {
	// Choose database implementation
	//shared_ptr<IDatabase> db = shared_ptr<IDatabase>(new InMemoryDatabase());
	shared_ptr<IDatabase> db = shared_ptr<IDatabase>(new SqliteDatabase());

	// Setup
	auto newsgroup1 = db->create_newsgroup("Newsgroup 1 title");
	auto article1 = db->create_article(newsgroup1->get_id(), "Awesome title 1", "Samme", "nope");
	auto article2 = db->create_article(newsgroup1->get_id(), "Bad title 1", "Samme", "yep");

	auto newsgroup2 = db->create_newsgroup("Newsgroup 2 title");
	auto article3 = db->create_article(newsgroup2->get_id(), "Awesome title 2", "Samme", "nope");
	auto article4 = db->create_article(newsgroup2->get_id(), "Bad title 2", "Samme", "yep");

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

	string OK = " -- OK!", FAILED = " -- FAILED!";
	string test;

	// Newsgroup get test: Existing newsgroup
	test = "Newsgroup get test: Existing newsgroup";
	try {
		auto newsgroup = db->get_newsgroup(newsgroup1->get_id());
		if (newsgroup == nullptr)
			throw runtime_error("Returned newsgroup was null.");
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Newsgroup get test: Non-existing newsgroup
	test = "Newsgroup get test: Non-existing newsgroup";
	try {
		auto newsgroup = db->get_newsgroup(1337);
		throw runtime_error("No exception thrown.");
	}
	catch (group_not_found &) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Newsgroup list test
	test = "Newsgroup list test";
	try {
		auto ng_vec = db->list_newsgroups();
		if (ng_vec.size() != 2)
			throw runtime_error("Wrong size of newsgroup vector.");
		for (auto it = ng_vec.begin(); it != ng_vec.end(); ++it) {
			if (*it == nullptr)
				throw runtime_error("Newsgroup pointer in vector was null.");
		}
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Newsgroup creation test: Existing newsgroup
	test = "Newsgroup creation test: Existing newsgroup";
	try {
		db->create_newsgroup(newsgroup2->get_title());
		throw runtime_error("No exception thrown.");
	}
	catch (group_already_exists) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Newsgroup deletion test: Non-existing newsgroup
	test = "Newsgroup deletion test: Non-existing newsgroup";
	try {
		db->delete_newsgroup(1337);
		throw runtime_error("No exception thrown.");
	}
	catch (group_not_found &) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Newsgroup deletion test: Existing newsgroup
	test = "Newsgroup deletion test: Existing newsgroup";
	try {
		db->delete_newsgroup(newsgroup2->get_id());
		if (db->list_newsgroups().size() != 1)
			throw runtime_error("Newsgroup was not properly deleted.");
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Newsgroup get test: Non-existing newsgroup
	test = "Newsgroup get test: Non-existing newsgroup";
	try {
		auto newsgroup = db->get_newsgroup(1337);
		throw runtime_error("No exception thrown.");
	}
	catch (group_not_found &) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Article get test: Existing newsgroup
	test = "Article get test: Existing newsgroup";
	try {
		auto article = db->get_article(newsgroup1->get_id(), article1->get_id());
		if (article == nullptr)
			throw runtime_error("Returned article was null.");
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Article get test: Non-existing newsgroup
	test = "Article get test: Non-existing newsgroup";
	try {
		db->get_article(1337, article1->get_id());
		throw runtime_error("No exception thrown.");
	}
	catch (group_not_found &) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Article get test: Non-existing article
	test = "Article get test: Non-existing article";
	try {
		db->get_article(newsgroup1->get_id(), 1337);
		throw runtime_error("No exception thrown.");
	}
	catch (article_not_found &) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Article list test
	test = "Article list test";
	try {
		auto art_vec = db->list_articles(newsgroup1->get_id());
		if (art_vec.size() != 2)
			throw runtime_error("Wrong size of newsgroup vector.");
		for (auto it = art_vec.begin(); it != art_vec.end(); ++it) {
			if (*it == nullptr)
				throw runtime_error("Article pointer in vector was null.");
		}
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Article deletion test: Non-existing article
	test = "Article deletion test: Non-existing article";
	try {
		db->delete_article(newsgroup1->get_id(), 1337);
		throw runtime_error("No exception thrown.");
	}
	catch (article_not_found &e) {
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	// Article deletion test: Existing article
	test = "Article deletion test: Existing article";
	try {
		db->delete_article(newsgroup1->get_id(), article1->get_id());
		if (db->list_articles(newsgroup1->get_id()).size() != 1)
			throw runtime_error("Article was not properly deleted.");
		cout << test << OK << endl;
	}
	catch (exception &e) {
		cout << test << FAILED << endl;
		cout << e.what() << endl;
	}

	/*
	cout << endl << "Post-deletion printout:" << endl;
	printAll(db);
	*/
}

void printNewsgroup(shared_ptr<INewsgroup> newsgroup) {
	cout << "### Newsgroup ###" << endl;
	cout << "ID: " << newsgroup->get_id() << endl;
	cout << "Title: " << newsgroup->get_title() << endl;
	cout << endl;
}

void printArticle(shared_ptr<IArticle> article) {
	cout << "--- Article ---" << endl;
	cout << "ID: " << article->get_id() << endl;
	cout << "Title: " << article->get_title() << endl;
	cout << "Author: " << article->get_author() << endl;
	cout << "Text: " << article->get_text() << endl;
	cout << endl;
}

void printAll(shared_ptr<IDatabase> db) {
	auto ng_vec = db->list_newsgroups();
	for (auto ng_it = ng_vec.begin(); ng_it != ng_vec.end(); ++ng_it) {
		auto newsgroup = dynamic_pointer_cast<InMemoryNewsgroup>(*ng_it);
		printNewsgroup(newsgroup);
		auto art_vec = db->list_articles(newsgroup->get_id());
		for (auto art_it = art_vec.begin(); art_it != art_vec.end(); ++art_it) {
			printArticle(dynamic_pointer_cast<InMemoryArticle>(*art_it));
		}
		cout << string('-', 20) << endl;
	}
}
