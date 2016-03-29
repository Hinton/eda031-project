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

	std::vector<std::shared_ptr<INewsgroup>> list_newsgroups();
	std::shared_ptr<INewsgroup> get_newsgroup(const int& id);
	std::shared_ptr<INewsgroup> create_newsgroup(const std::string &title);
	bool delete_newsgroup(const int &id);

	std::vector<std::shared_ptr<IArticle>> list_articles(const int& newsgroup_id);
	std::shared_ptr<IArticle> get_article(const int &newsgroup_id, const int &article_id);
	std::shared_ptr<IArticle> create_article(const int &newsgroup_id,
																				const std::string &title,
																				const std::string &author, const std::string &text);

	bool delete_article(const int &newsgroup_id, const int &article_id);
	
	int convert_group_id(const std::string& id);
private: 
	std::shared_ptr<ServerCommunication> scom;
	bool is_number(const std::string& s);
};


#endif
