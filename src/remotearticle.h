#ifndef REMOTEARTICLE_H
#define REMOTEARTICLE_H

#include <string>
#include <memory>
#include "idatabase.h"
#include "inewsgroup.h"
#include "iarticle.h"
#include "remotedatabase.h"

class RemoteArticle : public IArticle {
public:
	RemoteArticle(const std::shared_ptr<ServerCommunication>& scom, int id, std::string title,
		std::string author, std::string text) 
	: scom(scom), id(id), title(title), author(author), text(text) {};
	~RemoteArticle() = default;

	int get_id();

	std::string get_title();
	void set_title(const std::string &title);

	std::string get_author();
	void set_author(const std::string &author);

	std::string get_text();
	void set_text(const std::string &text);
private:
	std::shared_ptr<ServerCommunication> scom;
	int id;
	std::string title;
	std::string author;
	std::string text;
};

#endif
