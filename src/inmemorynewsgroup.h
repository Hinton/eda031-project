#ifndef INMEMORYNEWSGROUP_H
#define INMEMORYNEWSGROUP_H

#include <map>
#include "inewsgroup.h"
#include "iarticle.h"

class InMemoryNewsgroup : INewsgroup {
public:
	InMemoryNewsgroup(const int &id, const std::string &title);

	virtual int get_id();
	//virtual void setId(const int &id);

	virtual std::string get_title();

	virtual void set_title(const std::string &title);

	std::shared_ptr<IArticle> add_article(const int &id, const std::string &title,
										  const std::string &author, const std::string &text);

	std::shared_ptr<IArticle> get_article(const int &id);

	std::vector<std::shared_ptr<IArticle>> get_articles();

	bool remove_article(const int &article_id);

private:
	std::map<int, IArticle> articles;
};

#endif
