#ifndef REMOTENEWSGROUP_H
#define REMOTENEWSGROUP_H

#include <string>
#include <memory>
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"
#include "remotedatabase.h"

class RemoteNewsgroup : public INewsgroup {
public:
	RemoteNewsgroup(ServerCommunication *scom, RemoteDatabase *rdb, int id, std::string title) : scom(scom), rdb(rdb), id(id), title(title) {};
//	~RemoteNewsgroup() = default;

	int get_id();

	std::string get_title();
	void set_title(const std::string &title);

	std::shared_ptr<IArticle> get_article(const int &id);
	std::shared_ptr<IArticle> create_article(const std::string &title,
		const std::string &author, const std::string &text);

	std::vector<std::shared_ptr<IArticle>> list_articles();
	bool remove_article(const int &id);

private:
	ServerCommunication *scom;
	RemoteDatabase *rdb;
	int id;
	std::string title;
};

#endif
