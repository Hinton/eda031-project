#ifndef INMEMORYNEWSGROUP_H
#define INMEMORYNEWSGROUP_H

#include <map>
#include <set>
#include "inewsgroup.h"
#include "iarticle.h"

class InMemoryNewsgroup : INewsgroup {
public:
	InMemoryNewsgroup(const int &id, const std::string &title);

	int get_id() { return id; }
	//virtual void setId(const int &id);

	std::string get_title() { return title; }

	void set_title(const std::string &title) { this->title = title; }

	std::shared_ptr<IArticle> add_article(const int &id, const std::string &title,
										  const std::string &author, const std::string &text);

	std::shared_ptr<IArticle> get_article(const int &id);

	std::vector<std::shared_ptr<IArticle>> get_articles();

	bool remove_article(const int &id);

private:
	const int id;
	std::string title;
	std::map<int, std::shared_ptr<IArticle>> articles;
	std::set<int> used_article_ids;
};

#endif
