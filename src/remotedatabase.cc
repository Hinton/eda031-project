#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"
#include "remotedatabase.h"
#include "remotenewsgroup.h"
#include "remotearticle.h"
#include "database_exceptions.h"

using namespace std;

vector<shared_ptr<INewsgroup>> RemoteDatabase::list_newsgroups() {
	vector<pair<int, string>> ngrps = scom->list_newsgroups();
	vector<shared_ptr<INewsgroup>> to_return(ngrps.size());
	transform (ngrps.begin(), ngrps.end(), to_return.begin(), [&](pair<int, string> grp) { 
		return shared_ptr<RemoteNewsgroup>(new RemoteNewsgroup(scom, shared_ptr<RemoteDatabase>(this), grp.first, grp.second)); });
	return to_return;
}

shared_ptr<INewsgroup> RemoteDatabase::get_newsgroup(const int& id) {
	throw func_not_supported();
}

shared_ptr<INewsgroup> RemoteDatabase::create_newsgroup(const string &title) {
	pair<bool, int> res = scom->create_newsgroup(title);
	if (!res.first) {
		throw obj_already_exists();
	} else {
		return shared_ptr<INewsgroup>(new RemoteNewsgroup(scom, shared_ptr<RemoteDatabase>(this), res.second, title));
	}
}

bool RemoteDatabase::delete_newsgroup(const int &id) {
	//TODO: Not use to_string() ?
	return scom->delete_newsgroup(to_string(id));
}

vector<shared_ptr<IArticle>> RemoteDatabase::list_articles(const int& newsgroup_id) {
	//pair<group number, pair<article number, article string>>
	//pair<int, vector<pair<int, string>>> res = scom->list_articles(newsgroup_id);
	throw func_not_implemented();
}

shared_ptr<IArticle> RemoteDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	throw func_not_supported();
}

shared_ptr<IArticle> RemoteDatabase::create_article(const int &newsgroup_id,
	const string &title, const string &author, const string &text) {	
	if (!create_article(newsgroup_id, title, author, text)) {
		throw obj_already_exists();
	} else {
		return shared_ptr<IArticle>(new RemoteArticle(scom, newsgroup_id, title, author, text));
	}
}


bool RemoteDatabase::delete_article(const int &newsgroup_id, const int &article_id) {
	return scom->delete_article(newsgroup_id, article_id) == "Success";
}
