#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#define DB_FILENAME "file:newsserver.db"

#include <memory>
#include <exception>
#include <fstream>
#include "../sqlite_src/sqlite3.h"
#include "idatabase.h"
#include "inewsgroup.h"

//class SqliteNewsgroup;

class SqliteDatabase : public IDatabase {
	using article_vec = std::vector<std::shared_ptr<IArticle>>;
	using newsgroup_vec = std::vector<std::shared_ptr<INewsgroup>>;
public:
	SqliteDatabase();

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
	sqlite3* db;

	inline bool file_exists(const std::string& name) {
		if (std::ifstream(name))
		{
			return false;
		}
		std::ofstream file(name);
		if (!file)
		{
			return false;
		}
	};
};


#endif
