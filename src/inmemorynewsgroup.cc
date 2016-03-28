#include <memory>
#include "inmemorynewsgroup.h"
#include "inmemoryarticle.h"
#include "idusedexception.h"

using namespace std;

InMemoryNewsgroup::InMemoryNewsgroup(const int &id, const std::string &title) : id(id), title(title) { }

std::shared_ptr<IArticle> InMemoryNewsgroup::add_article(const int &id, const std::string &title,
														 const std::string &author, const std::string &text) {
	if (used_article_ids.find(id) == used_article_ids.end()) {
		articles.insert({id, shared_ptr<InMemoryArticle>(new InMemoryArticle(id, title, author, text))});
		used_article_ids.insert(id);
		return dynamic_pointer_cast<IArticle>(articles.at(id));
	}
	else {
		throw IDUsedException("Article ID already used.");
	}
}

std::shared_ptr<IArticle> InMemoryNewsgroup::get_article(const int &id) {
	return dynamic_pointer_cast<IArticle>(articles.at(id));
}

InMemoryNewsgroup::article_vec InMemoryNewsgroup::get_articles() {
	vector<shared_ptr<IArticle>> ret;
	for (auto it = articles.begin(); it != articles.end(); ++it) {
		ret.push_back(dynamic_pointer_cast<IArticle>(it->second));
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
