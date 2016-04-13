#ifndef SQLITEARTICLE_H
#define SQLITEARTICLE_H

#include <string>
#include "idatabase.h"
#include "sqlitedatabase.h"
#include "inewsgroup.h"
#include "sqlitenewsgroup.h"
#include "iarticle.h"
#include "sqlitearticle.h"
#include "../sqlite_src/sqlite3.h"

class SqliteArticle : public IArticle {
public:
	SqliteArticle(SqliteDatabase* db, const int &id)
		: db(db), id(id) {};

	int get_id();

	std::string get_title();

	void set_title(const std::string &title);

	std::string get_author();

	void set_author(const std::string &author);

	std::string get_text();

	void set_text(const std::string &text);

private:
	SqliteDatabase* db;
	int id;

	std::string get_column(std::string column);
	void set_column(std::string column, std::string value);
};

#endif
