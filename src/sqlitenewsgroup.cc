#include <sstream>
#include "sqlitenewsgroup.h"

using namespace std;

std::string SqliteNewsgroup::get_title() {
	stringstream ss;
	ss << "SELECT title FROM newsgroups WHERE id = " << id;
	string title;
	sqlite3_exec(db, ss.str().c_str(), [](void* arg, int argc, char** argv, char** col_names) -> int {
		*(string*)arg = argv[0];
		return 0;
	}, &title, nullptr);
	return title;
}

void SqliteNewsgroup::set_title(const std::string &title) {
	stringstream ss;
	ss << "UPDATE newsgroups SET title = '" << title << "' WHERE id = " << id;
	sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

std::shared_ptr<IArticle> SqliteNewsgroup::create_article(const std::string &title,
										 const std::string &author, const std::string &text) {

}

std::shared_ptr<IArticle> SqliteNewsgroup::get_article(const int &id) {

}

std::vector<std::shared_ptr<IArticle>> SqliteNewsgroup::list_articles() {

}

bool SqliteNewsgroup::remove_article(const int &id) {
	
}
