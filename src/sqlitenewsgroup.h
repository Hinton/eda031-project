#ifndef SQLITENEWSGROUP_H
#define SQLITENEWSGROUP_H

#include "inewsgroup.h"
#include "iarticle.h"
#include "../sqlite_src/sqlite3.h"

class SqliteNewsgroup : public INewsgroup {
	using article_vec = std::vector<std::shared_ptr<IArticle>>;
public:
	SqliteNewsgroup(sqlite3 *db, const int &id) : db(db), id(id) {};

	int get_id() { return id; };

	std::string get_title();

	void set_title(const std::string &title);

	std::shared_ptr<IArticle> create_article(const std::string &title,
											 const std::string &author, const std::string &text);

	std::shared_ptr<IArticle> get_article(const int &id);

	article_vec list_articles();

	bool remove_article(const int &id);

private:
	sqlite3* db;
	int id;
};

#endif
