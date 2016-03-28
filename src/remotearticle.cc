#include <string>
#include <memory>
#include "database_exceptions.h"
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"
#include "remotedatabase.h"
#include "remotearticle.h"

using namespace std;

int RemoteArticle::get_id() {
	return id;
}

string RemoteArticle::get_title() {
	return title;
}

void RemoteArticle::set_title(const string &title) {
	throw func_not_supported();
}

string RemoteArticle::get_author() {
	return author;
}

void RemoteArticle::set_author(const string &author) {
	throw func_not_supported();
}

string RemoteArticle::get_text() {
	return text;
}

void RemoteArticle::set_text(const string &text) {
	throw func_not_supported();
}
