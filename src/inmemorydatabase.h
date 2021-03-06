#ifndef INMEMORYDATABASE_H
#define INMEMORYDATABASE_H

#include <map>
#include <set>
#include "idatabase.h"
#include "inewsgroup.h"

class InMemoryNewsgroup;

class InMemoryDatabase : public IDatabase {
	friend class InMemoryNewsgroup;
	using article_vec = std::vector<std::shared_ptr<IArticle>>;
	using newsgroup_vec = std::vector<std::shared_ptr<INewsgroup>>;
public:
	InMemoryDatabase() : free_newsgroup_id(0) { }

	newsgroup_vec list_newsgroups();

	std::shared_ptr<INewsgroup> get_newsgroup(const int &id);

	std::shared_ptr<INewsgroup> create_newsgroup(const std::string &title);

	bool delete_newsgroup(const int &id);

	article_vec list_articles(const int &newsgroup_id);

	std::shared_ptr<IArticle> get_article(const int &newsgroup_id, const int &article_id);

	std::shared_ptr<IArticle> create_article(const int &newsgroup_id, const std::string &title,
											 const std::string &author, const std::string &text);

	bool delete_article(const int &newsgroup_id, const int &article_id);

private:
	int new_newsgroup_id() { return free_newsgroup_id++; }

	int free_newsgroup_id;

	using newsgroup_map = std::map<int, std::shared_ptr<InMemoryNewsgroup>>;
	newsgroup_map newsgroups;

	std::set<std::string> newsgroup_titles;
};


#endif
