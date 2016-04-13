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
				"title TEXT, "
				"CONSTRAINT unq_title UNIQUE (title)"
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
	db_exec("SELECT last_insert_rowid()", [&](SqliteLambdaOutput &out) {
		id = atoi(out.argv[0]);
	});
	return id;
}

vector<shared_ptr<INewsgroup>> SqliteDatabase::list_newsgroups() {
	vector<shared_ptr<INewsgroup>> to_return;

	db_exec("SELECT id FROM newsgroups", [&](SqliteLambdaOutput &out) {
		to_return.push_back(shared_ptr<INewsgroup>(new SqliteNewsgroup(this, atoi(out.argv[0]))));
	});

	return to_return;
}

shared_ptr<INewsgroup> SqliteDatabase::get_newsgroup(const int &id) {
	stringstream ss;
	ss << "SELECT COUNT(*) FROM newsgroups WHERE id = " << id;
	int count(0);
	db_exec(ss.str(), [&](SqliteLambdaOutput &out) {
		count = atoi(out.argv[0]);
	});

	if (count != 1) {
		throw group_not_found(); 
	}

	return shared_ptr<INewsgroup>(new SqliteNewsgroup(this, id));
}

shared_ptr<INewsgroup> SqliteDatabase::create_newsgroup(const string &title) {
	stringstream ss;
	ss << "INSERT INTO newsgroups (title) VALUES ('" << title << "')";
	try {
		db_exec(ss.str());
	} catch (runtime_error err) {
		if (string(err.what()).find("tjosan")) {

		} else {
			throw err;
		}
	}

	return shared_ptr<INewsgroup>(new SqliteNewsgroup(this, last_insert_id()));
}

bool SqliteDatabase::delete_newsgroup(const int &id) {
	get_newsgroup(id);
	stringstream ss;
	ss << "DELETE FROM newsgroups WHERE id = " << id;
	db_exec(ss.str());
}

SqliteDatabase::article_vec SqliteDatabase::list_articles(const int &newsgroup_id) {
	return get_newsgroup(newsgroup_id)->list_articles();
}

shared_ptr<IArticle> SqliteDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	return get_newsgroup(newsgroup_id)->get_article(article_id);
}

shared_ptr<IArticle> SqliteDatabase::create_article(const int &newsgroup_id, const string &title,
										 const string &author, const string &text) {
	return get_newsgroup(newsgroup_id)->create_article(title, author, text);
} 

bool SqliteDatabase::delete_article(const int &newsgroup_id, const int &article_id) {
	return get_newsgroup(newsgroup_id)->remove_article(article_id);
}