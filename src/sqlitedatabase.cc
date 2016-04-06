#include <memory>
#include <exception>
#include <string>
#include "sqlitedatabase.h"
#include "sqlitenewsgroup.h"
#include "sqlitearticle.h"
#include "../sqlite_src/sqlite3.h"

using namespace std;

SqliteDatabase::SqliteDatabase() {
	bool need_to_create_tables = file_exists(DB_FILENAME);

	int res = sqlite3_open(
		DB_FILENAME,   /* Database filename (UTF-8) */
		&db          /* OUT: SQLite db handle */
	);

	if (res != SQLITE_OK) {
		throw runtime_error("Couldn't open database: " + string(sqlite3_errmsg(db)) + "\nGonna crash now, have a good day!");
	}

	if (need_to_create_tables) {
		const char *statements[1];
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

SqliteDatabase::newsgroup_vec SqliteDatabase::list_newsgroups() {

}

std::shared_ptr<INewsgroup> SqliteDatabase::get_newsgroup(const int &id) {

}

std::shared_ptr<INewsgroup> SqliteDatabase::create_newsgroup(const std::string &title) {

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
