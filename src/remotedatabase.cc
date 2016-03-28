#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"
#include "remotedatabase.h"
#include "remotenewsgroup.h"
#include "database_exceptions.h"

using namespace std;

vector<shared_ptr<INewsgroup>> RemoteDatabase::get_newsgroups() {
	vector<pair<int, string>> ngrps = scom->list_newsgroups();
	vector<shared_ptr<INewsgroup>> to_return(ngrps.size());
	transform (ngrps.begin(), ngrps.end(), to_return.begin(), [&](pair<int, string> grp) { 
		return shared_ptr<RemoteNewsgroup>(new RemoteNewsgroup(scom, grp.first, grp.second)); });
	return to_return;
}

shared_ptr<INewsgroup> RemoteDatabase::get_newsgroup(const int& id) {
	throw func_not_implemented();
}

shared_ptr<INewsgroup> RemoteDatabase::add_newsgroup(const int& id, const string &title) {
	throw func_not_implemented();	
}

bool RemoteDatabase::remove_newsgroup(const int &id) {
	throw func_not_implemented();
}

vector<shared_ptr<IArticle>> RemoteDatabase::get_articles(const int& newsgroup_id) {
	throw func_not_implemented();
}

shared_ptr<IArticle> RemoteDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	throw func_not_implemented();
}

shared_ptr<IArticle> RemoteDatabase::add_article(const int &newsgroup_id, const int &article_id,
	const string &title, const string &author, const string &text) {	
	throw func_not_implemented();
}


bool RemoteDatabase::remove_article(const int &newsgroup_id, const int &article_id) {
	throw func_not_implemented();
}
