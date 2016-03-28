#ifndef INMEMORYDATABASE_H
#define INMEMORYDATABASE_H

#include <map>
#include <set>
#include "idatabase.h"
#include "inewsgroup.h"

class InMemoryNewsgroup;

class InMemoryDatabase : public IDatabase {
	using article_vec = std::vector<std::shared_ptr<IArticle>>;
	using newsgroup_vec = std::vector<std::shared_ptr<INewsgroup>>;
public:
	newsgroup_vec get_newsgroups();

	std::shared_ptr<INewsgroup> get_newsgroup(const int &id);

	std::shared_ptr<INewsgroup> add_newsgroup(const int &id, const std::string &title);

	bool remove_newsgroup(const int &id);

	article_vec get_articles(const int &newsgroup_id);

	std::shared_ptr<IArticle> get_article(const int &newsgroup_id, const int &article_id);

	std::shared_ptr<IArticle> add_article(const int &newsgroup_id, const int &article_id, const std::string &title,
										  const std::string &author, const std::string &text);

	bool remove_article(const int &newsgroup_id, const int &article_id);

private:
	using newsgroup_map = std::map<int, std::shared_ptr<InMemoryNewsgroup>>;

	std::set<int> used_newsgroup_ids;
	newsgroup_map newsgroups;
};


#endif
