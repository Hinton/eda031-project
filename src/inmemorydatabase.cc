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
	return dynamic_pointer_cast<INewsgroup>(newsgroups.at(id));
}

std::shared_ptr<INewsgroup> InMemoryDatabase::create_newsgroup(const std::string &title) {
	int id = get_newsgroup_id();
	newsgroups.insert({id, shared_ptr<InMemoryNewsgroup>(new InMemoryNewsgroup(id, title))});
	return dynamic_pointer_cast<INewsgroup>(newsgroups.at(id));
}

bool InMemoryDatabase::delete_newsgroup(const int &id) {
	bool success = false;
	if (newsgroups.find(id) != newsgroups.end()) {
		newsgroups.erase(id);
		success = true;
	}
	return success;
}

InMemoryDatabase::article_vec InMemoryDatabase::list_articles(const int &newsgroup_id) {
	return newsgroups.at(newsgroup_id)->list_articles();
}

std::shared_ptr<IArticle> InMemoryDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	return newsgroups.at(newsgroup_id)->get_article(article_id);
}

std::shared_ptr<IArticle> InMemoryDatabase::create_article(const int &newsgroup_id, const std::string &title,
														   const std::string &author, const std::string &text) {

	return newsgroups.at(newsgroup_id)->create_article(title, author, text);
}

bool InMemoryDatabase::delete_article(const int &newsgroup_id, const int &article_id) {
	return newsgroups.at(newsgroup_id)->delete_article(article_id);
}

