#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#define DB_FILENAME "newsserver.db"

#include <memory>
#include <exception>
#include <iostream>
#include <fstream>
#include "../sqlite_src/sqlite3.h"
#include "idatabase.h"
#include "inewsgroup.h"

//class SqliteNewsgroup;

class SqliteDatabase : public IDatabase {
	friend class SqliteNewsgroup;
	friend class SqliteArticle;

	using article_vec = std::vector<std::shared_ptr<IArticle>>;
	using newsgroup_vec = std::vector<std::shared_ptr<INewsgroup>>;
public:
	SqliteDatabase();
	~SqliteDatabase();

	newsgroup_vec list_newsgroups();

	std::shared_ptr<INewsgroup> get_newsgroup(const int &id);

	std::shared_ptr<INewsgroup> create_newsgroup(const std::string &title);

	bool delete_newsgroup(const int &id);

	article_vec list_articles(const int &newsgroup_id);

	std::shared_ptr<IArticle> get_article(const int &newsgroup_id, const int &article_id);

	std::shared_ptr<IArticle> create_article(const int &newsgroup_id, const std::string &title,
											 const std::string &author, const std::string &text);

	bool delete_article(const int &newsgroup_id, const int &article_id);

	template <typename F>
	void db_exec(const std::string &sql, F callback) {
		std::cout << "[DATABASE] " << sql << std::endl;
		char *errmsg = nullptr;
		int res = sqlite3_exec(db, sql.c_str(), [](void* arg, int argc, char** argv, char** col_names) -> int {
			F callback = *(F*)arg;
			SqliteLambdaOutput out;
			out.argc = argc;
			out.argv = argv;
			out.col_names = col_names;

			callback(out);

			return 0;
		}, &callback, &errmsg);

		if (errmsg != nullptr) {
			throw std::runtime_error(errmsg);
		}

		if (res != 0) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}
	}

	void db_exec(const std::string &sql) {
		std::cout << "[DATABASE] " << sql << std::endl;
		char *errmsg = nullptr;
		int res = sqlite3_exec(db, sql.c_str(), [](void* arg, int argc, char** argv, char** col_names) -> int {
			SqliteLambdaOutput out;
			out.argc = argc;
			out.argv = argv;
			out.col_names = col_names;
			return 0;
		}, nullptr, &errmsg);

		if (errmsg != nullptr) {
			throw std::runtime_error(errmsg);
		}

		if (res != 0) {
			throw std::runtime_error(sqlite3_errmsg(db));
		}
	}

	struct SqliteLambdaOutput
	{
		int argc;
		char** argv; 
		char** col_names;
	};

private:
	sqlite3* db;
	inline bool file_exists(const std::string& name) {
		std::ifstream ifile(name);
  	return ifile.is_open();
	};

	int last_insert_id();
};


#endif
