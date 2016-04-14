#include <string>
#include <memory>
#include "database_exceptions.h"
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"
#include "remotedatabase.h"
#include "remotenewsgroup.h"

using namespace std;

int RemoteNewsgroup::get_id() {
	return id;
}

string RemoteNewsgroup::get_title() {
	return title;
}

void RemoteNewsgroup::set_title(const std::string &) {
	throw func_not_supported();
}

std::shared_ptr<IArticle> RemoteNewsgroup::get_article(const int &) {
	throw func_not_supported();
}

std::shared_ptr<IArticle> RemoteNewsgroup::create_article(const std::string &title,
	const std::string &author, const std::string &text) {
	return rdb->create_article(this->id, title, author, text);
}

std::vector<std::shared_ptr<IArticle>> RemoteNewsgroup::list_articles() {
	return rdb->list_articles(this->id);
}

bool RemoteNewsgroup::remove_article(const int &article_id) {
	return rdb->delete_article(this->id, article_id);
}
