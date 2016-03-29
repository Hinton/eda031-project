#include <memory>
#include "database_exceptions.h"
#include "inmemorynewsgroup.h"
#include "inmemoryarticle.h"

using namespace std;

InMemoryNewsgroup::InMemoryNewsgroup(const int &id, const std::string &title) : id(id), title(title) { }

std::shared_ptr<IArticle> InMemoryNewsgroup::create_article(const std::string &title,
															const std::string &author, const std::string &text) {
	int id = new_article_id();
	articles.insert({id, shared_ptr<InMemoryArticle>(new InMemoryArticle(id, title, author, text))});
	return dynamic_pointer_cast<IArticle>(articles.at(id));
}

std::shared_ptr<IArticle> InMemoryNewsgroup::get_article(const int &id) {
	try {
		return dynamic_pointer_cast<IArticle>(articles.at(id));
	}
	catch (out_of_range) {
		throw article_not_found();
	}
}

InMemoryNewsgroup::article_vec InMemoryNewsgroup::list_articles() {
	vector<shared_ptr<IArticle>> ret;
	for (auto it = articles.begin(); it != articles.end(); ++it) {
		ret.push_back(dynamic_pointer_cast<IArticle>(it->second));
	}
	return ret;
}

bool InMemoryNewsgroup::remove_article(const int &id) {
	bool success = false; // Not used, change to void?
	if (articles.find(id) != articles.end()) {
		articles.erase(id);
		success = true;
	}
	else {
		throw article_not_found();
	}
	return success;
}
