#include <memory>
#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include "sqlitedatabase.h"
#include "sqlitenewsgroup.h"
#include "sqlitearticle.h"
#include "../sqlite_src/sqlite3.h"
#include "database_exceptions.h"

using namespace std;

SqliteDatabase::SqliteDatabase() {
	bool need_to_create_tables = !file_exists(DB_FILENAME);

	int res = sqlite3_open(
		DB_FILENAME,   /* Database filename (UTF-8) */
		&db          /* OUT: SQLite db handle */
	);

	if (res != SQLITE_OK) {
		throw runtime_error("Couldn't open database: " + string(sqlite3_errmsg(db)) + "\nGonna crash now, have a good day!");
	}

	if (need_to_create_tables) {
		const char *statements[2];
		statements[0] = 
			"CREATE TABLE newsgroups ("
				"id INTEGER PRIMARY KEY, "
				"title TEXT"
			");";

		statements[1] = 
			"CREATE TABLE articles ("
				"id INTEGER PRIMARY KEY, "
				"newsgroup_id INTEGER, "
				"title TEXT, "
				"author TEXT, "
				"text TEXT, "
				"FOREIGN KEY (newsgroup_id) REFERENCES newsgroups(id)"
			");";
		
		for (const char **it = begin(statements); it != end(statements); ++it) {
			const char *stmt = *it;
			sqlite3_stmt *prepared_stmt;
			res = sqlite3_prepare(db, stmt, 200, &prepared_stmt, nullptr);

			res = sqlite3_step(prepared_stmt);
			if (res != SQLITE_DONE) {
				throw runtime_error("Couldn't execute query: " + string(sqlite3_errmsg(db)) + "\nGonna crash now, have a good day!");
			}

			sqlite3_finalize(prepared_stmt);
		}
	}
}

SqliteDatabase::~SqliteDatabase() {
	sqlite3_close(db);
}

int SqliteDatabase::last_insert_id() {
	int id;
	sqlite3_exec(db, "SELECT last_insert_rowid()", [](void* arg, int argc, char** argv, char** col_names) -> int {
		*(int*)arg = *argv[0];
		return 0;
	}, &id, nullptr);
	return id;
}

std::vector<std::shared_ptr<INewsgroup>> SqliteDatabase::list_newsgroups() {
	std::vector<int> ids;
	
	sqlite3_exec(db, "SELECT id FROM newsgroups", [](void* arg, int argc, char** argv, char** col_names) -> int {
		((std::vector<int> *)arg)->push_back(*argv[0]);
		return 0;
	}, &ids, nullptr);
	
	std::vector<std::shared_ptr<INewsgroup>> out;
	for (int id : ids) {
		out.emplace_back(new SqliteNewsgroup(db, id));
	}
	return out;
}

std::shared_ptr<INewsgroup> SqliteDatabase::get_newsgroup(const int &id) {
	stringstream ss;
	ss << "SELECT COUNT(*) FROM newsgroups WHERE id = " << id;
	int count(0);
	sqlite3_exec(db, ss.str().c_str(), [](void* arg, int argc, char** argv, char** col_names) -> int {
		*(int*)arg = *argv[0];
		return 0;
	}, &count, nullptr);

	if (count != 1) {
		throw group_not_found(); 
	}

	return shared_ptr<INewsgroup>(new SqliteNewsgroup(db, id));
}

std::shared_ptr<INewsgroup> SqliteDatabase::create_newsgroup(const std::string &title) {
	stringstream ss;
	ss << "INSERT INTO newsgroups (title) VALUES (" << title << ")";
	sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
	return shared_ptr<INewsgroup>(new SqliteNewsgroup(db, last_insert_id()));
}

bool SqliteDatabase::delete_newsgroup(const int &id) {

}

SqliteDatabase::article_vec SqliteDatabase::list_articles(const int &newsgroup_id) {

}

std::shared_ptr<IArticle> SqliteDatabase::get_article(const int &newsgroup_id, const int &article_id) {

}

std::shared_ptr<IArticle> SqliteDatabase::create_article(const int &newsgroup_id, const std::string &title,
										 const std::string &author, const std::string &text) {

} 

bool SqliteDatabase::delete_article(const int &newsgroup_id, const int &article_id) {

}
