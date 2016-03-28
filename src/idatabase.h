#ifndef IDATABASE_H
#define IDATABASE_H

#include <vector>
#include <memory>
#include "inewsgroup.h"
#include "iarticle.h"

class IDatabase {
public:
	// ~IDatabase = 0; funkar ej, se
	// http://www.geeksforgeeks.org/pure-virtual-destructor-c/
	virtual ~IDatabase() { };

	virtual std::vector<std::shared_ptr<INewsgroup>> get_newsgroups() = 0;

	virtual std::shared_ptr<INewsgroup> get_newsgroup(const int &id) = 0;

	virtual std::shared_ptr<INewsgroup> add_newsgroup(const int &id, const std::string &title) = 0;

	virtual bool remove_newsgroup(const int &id) = 0;

	virtual std::vector<std::shared_ptr<IArticle>> get_articles(const int &newsgroup_id) = 0;

	virtual std::shared_ptr<IArticle> get_article(const int &newsgroup_id, const int &article_id) = 0;

	virtual std::shared_ptr<IArticle> add_article(const int &newsgroup_id, const int &article_id,
												  const std::string &title,
												  const std::string &author, const std::string &text) = 0;

	virtual bool remove_article(const int &newsgroup_id, const int &article_id) = 0;

protected:
	IDatabase() { };
};

#endif
