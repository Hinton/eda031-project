#include "inmemorydatabase.h"
#include "inmemorynewsgroup.h"
#include "idusedexception.h"

using namespace std;

InMemoryDatabase::newsgroup_vec InMemoryDatabase::get_newsgroups() {
	newsgroup_vec ret;
	for (auto it = newsgroups.begin(); it != newsgroups.end(); ++it) {
		ret.push_back(dynamic_pointer_cast<INewsgroup>(it->second));
	}
	return ret;
}

std::shared_ptr<INewsgroup> InMemoryDatabase::get_newsgroup(const int &id) {
	return dynamic_pointer_cast<INewsgroup>(newsgroups.at(id));
}

std::shared_ptr<INewsgroup> InMemoryDatabase::add_newsgroup(const int &id, const std::string &title) {
	if (used_newsgroup_ids.find(id) == used_newsgroup_ids.end()) {
		newsgroups.insert({id, shared_ptr<InMemoryNewsgroup>(new InMemoryNewsgroup(id, title))});
		used_newsgroup_ids.insert(id);
		return dynamic_pointer_cast<INewsgroup>(newsgroups.at(id));
	}
	else {
		throw IDUsedException("Newsgroup ID already used.");
	}
}

bool InMemoryDatabase::remove_newsgroup(const int &id) {
	bool success = false;
	if (newsgroups.find(id) != newsgroups.end()) {
		newsgroups.erase(id);
		success = true;
	}
	return success;
}

InMemoryDatabase::article_vec InMemoryDatabase::get_articles(const int &newsgroup_id) {
	return newsgroups.at(newsgroup_id)->get_articles();
}

std::shared_ptr<IArticle> InMemoryDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	return newsgroups.at(newsgroup_id)->get_article(article_id);
}

std::shared_ptr<IArticle> InMemoryDatabase::add_article(const int &newsgroup_id, const int &article_id,
														const std::string &title, const std::string &author,
														const std::string &text) {
	return newsgroups.at(newsgroup_id)->add_article(article_id, title, author, text);
}

bool InMemoryDatabase::remove_article(const int &newsgroup_id, const int &article_id) {
	return newsgroups.at(newsgroup_id)->remove_article(article_id);
}

