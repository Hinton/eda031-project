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
		return shared_ptr<RemoteNewsgroup>(new RemoteNewsgroup(scom, this, grp.first, grp.second)); });
	return to_return;
}

shared_ptr<INewsgroup> RemoteDatabase::get_newsgroup(const int&) {
	throw func_not_supported();
}

shared_ptr<INewsgroup> RemoteDatabase::create_newsgroup(const string &title) {
	scom->create_newsgroup(title);
	return shared_ptr<INewsgroup>(new RemoteNewsgroup(scom, this, convert_group_id(title), title));
}

bool RemoteDatabase::delete_newsgroup(const int &id) {
	return scom->delete_newsgroup(id);
}

vector<shared_ptr<IArticle>> RemoteDatabase::list_articles(const int& newsgroup_id) {
	vector<pair<int, string>> articles = scom->list_articles(newsgroup_id);
	vector<shared_ptr<IArticle>> to_return(articles.size());
	transform (articles.begin(), articles.end(), to_return.begin(), [&](pair<int, string> article) { 
		return shared_ptr<RemoteArticle>(new RemoteArticle(scom, article.first, article.second, "", "")); });
	return to_return;
}

shared_ptr<IArticle> RemoteDatabase::get_article(const int &newsgroup_id, const int &article_id) {
	vector<string> res = scom->get_article(newsgroup_id, article_id);
	return shared_ptr<RemoteArticle>(new RemoteArticle(scom, article_id, res[0], res[1], res[2]));
}

shared_ptr<IArticle> RemoteDatabase::create_article(const int &newsgroup_id,
	const string &title, const string &author, const string &text) {	
	scom->create_article(newsgroup_id, title, author, text);
	return shared_ptr<IArticle>(new RemoteArticle(scom, newsgroup_id, title, author, text));
}


bool RemoteDatabase::delete_article(const int &newsgroup_id, const int &article_id) {
	return scom->delete_article(newsgroup_id, article_id);
}

int RemoteDatabase::convert_group_id(const string& id) {
	int group_nbr = -1;
	if (is_number(id)) {
		group_nbr = stoi(id);
	} else {
		auto groups = list_newsgroups();
		auto res = find_if(groups.begin(), groups.end(), [id] (shared_ptr<INewsgroup> p) { return p->get_title() == id; });
		if (res != groups.end()) {
			group_nbr = (*res)->get_id();
		}
	}
	return group_nbr;
}

bool RemoteDatabase::is_number(const string& s)  {
	return all_of(s.cbegin(), s.cend() , [] (char c)  { return isdigit(c); });
}
