#include <sstream>
#include "idatabase.h"
#include "sqlitedatabase.h"
#include "inewsgroup.h"
#include "sqlitenewsgroup.h"
#include "iarticle.h"
#include "sqlitearticle.h"
#include "database_exceptions.h"

using namespace std;

std::string SqliteNewsgroup::get_title() {
	stringstream ss;
	ss << "SELECT title FROM newsgroups WHERE id = " << id;
	string title;
	db->db_exec(ss.str(), [&](SqliteDatabase::SqliteLambdaOutput &out) {
		title = out.argv[0];
	});
	return title;
}

void SqliteNewsgroup::set_title(const std::string &title) {
	stringstream ss;
	ss << "UPDATE newsgroups SET title = '" << title << "' WHERE id = " << id;
	db->db_exec(ss.str());
}

std::shared_ptr<IArticle> SqliteNewsgroup::create_article(const std::string &title,
										 const std::string &author, const std::string &text) {
	stringstream ss;
	ss << "INSERT INTO articles (newsgroup_id, title, author, text) VALUES ("<< id << ", '" << title << "', '" << author << "', '" << text << "')";
	db->db_exec(ss.str());
	return shared_ptr<IArticle>(new SqliteArticle(db, db->last_insert_id()));
}

std::shared_ptr<IArticle> SqliteNewsgroup::get_article(const int &article_id) {
	stringstream ss;
	ss << "SELECT COUNT(*) FROM articles WHERE id = " << article_id;
	int count(0);
	db->db_exec(ss.str(), [&](SqliteDatabase::SqliteLambdaOutput &out) {
		count = atoi(out.argv[0]);
	});

	if (count != 1) {
		throw article_not_found(); 
	}

	return shared_ptr<IArticle>(new SqliteArticle(db, id));
}

std::vector<std::shared_ptr<IArticle>> SqliteNewsgroup::list_articles() {
	vector<shared_ptr<IArticle>> to_return;

	stringstream ss;
	ss << "SELECT id FROM articles WHERE newsgroup_id = " << id;
	db->db_exec(ss.str(), [&](SqliteDatabase::SqliteLambdaOutput &out) {
		to_return.push_back(shared_ptr<IArticle>(new SqliteArticle(db, atoi(out.argv[0]))));
	});

	return to_return;
}

bool SqliteNewsgroup::remove_article(const int &article_id) {
	get_article(id);
	stringstream ss;
	ss << "DELETE FROM articles WHERE id = " << article_id << " AND newsgroup_id = " << id;
	db->db_exec(ss.str());
}
