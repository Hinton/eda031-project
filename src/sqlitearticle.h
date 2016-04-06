#ifndef SQLITEARTICLE_H
#define SQLITEARTICLE_H

#include <string>
#include "iarticle.h"
#include "../sqlite_src/sqlite3.h"

class SqliteArticle : public IArticle {
public:
	SqliteArticle(sqlite3* db, const int &id, const std::string title, const std::string author, const std::string text)
		: db(db) {};

	int get_id();

	std::string get_title();

	void set_title(const std::string &title);

	std::string get_author();

	void set_author(const std::string &author);

	std::string get_text();

	void set_text(const std::string &text);

private:
	sqlite3* db;
};

#endif
