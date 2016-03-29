#include "database_exceptions.h"
#include "inmemorydatabase.h"
#include "inmemorynewsgroup.h"

using namespace std;

InMemoryDatabase::newsgroup_vec InMemoryDatabase::list_newsgroups() {
	newsgroup_vec ret;
	for (auto it = newsgroups.begin(); it != newsgroups.end(); ++it) {
		ret.push_back(dynamic_pointer_cast<INewsgroup>(it->second));
	}
	return ret;
}

std::shared_ptr<INewsgroup> InMemoryDatabase::get_newsgroup(const int &id) {
	try {
		return dynamic_pointer_cast<INewsgroup>(newsgroups.at(id));
	}
	catch (out_of_range) {
		throw group_not_found();
	}
}

std::shared_ptr<INewsgroup> InMemoryDatabase::create_newsgroup(const std::string &title) {
	if (newsgroup_titles.find(title) == newsgroup_titles.end()) {
		int id = new_newsgroup_id();
		newsgroups.insert({id, shared_ptr<InMemoryNewsgroup>(new InMemoryNewsgroup(id, title))});
		newsgroup_titles.insert(title);
		return dynamic_pointer_cast<INewsgroup>(newsgroups.at(id));
	}
	else {
		throw group_already_exists();
	}
}

bool InMemoryDatabase::delete_newsgroup(const int &id) {
	bool success = false; // Not used, change to void?
	if (newsgroups.find(id) != newsgroups.end()) {
		newsgroup_titles.erase(newsgroups.at(id)->get_title());
		newsgroups.erase(id);
		success = true;
	}
	else {
		throw group_not_found();
	}
	return success;
}

InMemoryDatabase::article_vec InMemoryDatabase::list_articles(const int &newsgroup_id) {
	try {
		return newsgroups.at(newsgroup_id)->list_articles();
	}
	catch (out_of_range) {
		throw group_not_found();
	}
}

std::shared_ptr<IArticle> InMemoryDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	try {
		return newsgroups.at(newsgroup_id)->get_article(article_id);
	}
	catch (out_of_range) {
		throw group_not_found();
	}
}

std::shared_ptr<IArticle> InMemoryDatabase::create_article(const int &newsgroup_id, const std::string &title,
														   const std::string &author, const std::string &text) {
	try {
		return newsgroups.at(newsgroup_id)->create_article(title, author, text);
	}
	catch (out_of_range) {
		throw group_not_found();
	}
}

bool InMemoryDatabase::delete_article(const int &newsgroup_id, const int &article_id) {
	try {
		return newsgroups.at(newsgroup_id)->remove_article(article_id);
	}
	catch (out_of_range) {
		throw group_not_found();
	}
}

