#include <sstream>
#include "idatabase.h"
#include "sqlitedatabase.h"
#include "inewsgroup.h"
#include "sqlitenewsgroup.h"
#include "iarticle.h"
#include "sqlitearticle.h"

using namespace std;

int SqliteArticle::get_id() {
	return id;
}

std::string SqliteArticle::get_title() {
	return get_column("title");
}

void SqliteArticle::set_title(const std::string &title) {
	set_column("title", title);
}

std::string SqliteArticle::get_author() {
	return get_column("author");
}

void SqliteArticle::set_author(const std::string &author) {
	set_column("author", author);
}

std::string SqliteArticle::get_text() {
	return get_column("text");
}

void SqliteArticle::set_text(const std::string &text) {
	set_column("text", text);
}

string SqliteArticle::get_column(string column) {
	string to_return;

	stringstream ss;
	ss << "SELECT "<< column <<" FROM articles WHERE id = " << id;
	db->db_exec(ss.str(), [&](SqliteDatabase::SqliteLambdaOutput &out) {
		to_return = out.argv[0];
	});

	return to_return;
}

void SqliteArticle::set_column(string column, string value) {
	stringstream ss;
	ss << "UPDATE articles SET "<< column <<" = '"<< value <<"' WHERE id = " << id;
	db->db_exec(ss.str());
}
