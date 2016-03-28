#ifndef REMOTEDATABASE_H
#define REMOTEDATABASE_H

#include <memory>
#include <vector>
#include "servercommunication.h"
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"

class RemoteDatabase : public IDatabase {
public: 
	RemoteDatabase(const std::shared_ptr<ServerCommunication>& scom) : scom(scom) {};
	~RemoteDatabase() {};

	std::vector<std::shared_ptr<INewsgroup>> get_newsgroups();
	std::shared_ptr<INewsgroup> get_newsgroup(const int& id);
	std::shared_ptr<INewsgroup> add_newsgroup(const int& id, const std::string &title);
	bool remove_newsgroup(const int &id);

	std::vector<std::shared_ptr<IArticle>> get_articles(const int& newsgroup_id);
	std::shared_ptr<IArticle> get_article(const int &newsgroup_id, const int &article_id);
	std::shared_ptr<IArticle> add_article(const int &newsgroup_id, const int &article_id,
																				const std::string &title,
																				const std::string &author, const std::string &text);

	bool remove_article(const int &newsgroup_id, const int &article_id);

private: 
	std::shared_ptr<ServerCommunication> scom;
};


#endif
