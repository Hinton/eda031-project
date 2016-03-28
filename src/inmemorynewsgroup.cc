#include "inmemorynewsgroup.h"
#include "inmemoryarticle.h"
#include "usedidexception.h"

using namespace std;

InMemoryNewsgroup::InMemoryNewsgroup(const int &id, const std::string &title) : id(id), title(title) { }

std::shared_ptr<IArticle> InMemoryNewsgroup::add_article(const int &id, const std::string &title,
														 const std::string &author, const std::string &text) {
	if (used_article_ids.find(id) == used_article_ids.end()) {
		articles.emplace(new InMemoryArticle(id, title, author, text));
		used_article_ids.insert(id);
		return articles.at(id);
	}
	else {
		throw UsedIDException("Article ID already used.");
	}
}

std::shared_ptr<IArticle> InMemoryNewsgroup::get_article(const int &id) {
	return articles.at(id);
}

std::vector<std::shared_ptr<IArticle>> InMemoryNewsgroup::get_articles() {
	vector<shared_ptr<IArticle>> ret;
	for (auto it = articles.begin(); it != articles.end(); ++it) {
		ret.push_back(it->second);
	}
	return ret;
}

bool InMemoryNewsgroup::remove_article(const int &id) {
	bool success = false;
	if (articles.find(id) != articles.end()) {
		articles.erase(id);
		success = true;
	}
	return success;
}
