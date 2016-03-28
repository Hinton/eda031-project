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

void RemoteNewsgroup::set_title(const std::string &title) {
	throw func_not_supported();
}

std::shared_ptr<IArticle> RemoteNewsgroup::get_article(const int &id) {
	//TODO: Implement
	throw func_not_implemented();
}

std::shared_ptr<IArticle> RemoteNewsgroup::add_article(const int &id, const std::string &title,
	const std::string &author, const std::string &text) {
	//TODO: Implement
	throw func_not_implemented();
}

std::vector<std::shared_ptr<IArticle>> RemoteNewsgroup::get_articles() {
	//TODO: Implement
	throw func_not_implemented();	
}

bool RemoteNewsgroup::remove_article(const int &article_id) {
	//TODO: Implement
	throw func_not_implemented();
}
