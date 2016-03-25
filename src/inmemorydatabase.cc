#include "inmemorydatabase.h"
#include "inmemorynewsgroup.h"

using namespace std;

vector<shared_ptr<INewsgroup>> InMemoryDatabase::get_newsgroups() {
	return vector<shared_ptr<INewsgroup>>(newsgroups.begin(), newsgroups.end());
}

std::shared_ptr<INewsgroup> InMemoryDatabase::get_newsgroup(const int &id) {
	return newsgroups.at(id);
}

std::shared_ptr<INewsgroup> InMemoryDatabase::add_newsgroup(const int &id, const std::string &title) {
	newsgroups.emplace(new InMemoryNewsgroup(id, title)); // Unsure if this use of emplace is ok
	return newsgroups.at(id);
}

bool InMemoryDatabase::remove_newsgroup(const int &id) {
	bool success = false;
	if (newsgroups.find(id) != newsgroups.end()) {
		newsgroups.erase(id);
		success = true;
	}
	return success;
}

std::vector<std::shared_ptr<IArticle>> InMemoryDatabase::get_articles(const int &newsgroup_id) {
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

