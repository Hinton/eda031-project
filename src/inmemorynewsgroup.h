#ifndef INMEMORYNEWSGROUP_H
#define INMEMORYNEWSGROUP_H

#include <map>
#include <set>
#include "inewsgroup.h"
#include "iarticle.h"

class InMemoryDatabase;
class InMemoryArticle;

class InMemoryNewsgroup : public INewsgroup {
	using article_vec = std::vector<std::shared_ptr<IArticle>>;
public:
	InMemoryNewsgroup(const int &id, const std::string &title);

	int get_id() { return id; }
	//virtual void setId(const int &id);

	std::string get_title() { return title; }

	void set_title(const std::string &title) { this->title = title; }

	std::shared_ptr<IArticle> create_article(const std::string &title,
											 const std::string &author, const std::string &text);

	std::shared_ptr<IArticle> get_article(const int &id);

	article_vec list_articles();

	bool delete_article(const int &id);

private:
	using article_map = std::map<int, std::shared_ptr<InMemoryArticle>>;

	int get_article_id() { return free_article_id++; }

	const int id;
	std::string title;
	article_map articles;
	int free_article_id;
};

#endif
