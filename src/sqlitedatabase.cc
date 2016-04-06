#include <memory>
#include "sqlitedatabase.h"

using namespace std;

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
