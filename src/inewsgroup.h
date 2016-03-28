#ifndef INEWSGROUP_H
#define INEWSGROUP_H

#include <string>
#include <vector>
#include <memory>
#include "iarticle.h"

class IArticle;
class INewsgroup {
public:
	virtual ~INewsgroup() { };

	virtual int get_id() = 0;
	//virtual void setId(const int &id) = 0;

	virtual std::string get_title() = 0;

	virtual void set_title(const std::string &title) = 0;

	virtual std::shared_ptr<IArticle> add_article(const int &id, const std::string &title,
												  const std::string &author, const std::string &text) = 0;

	virtual std::shared_ptr<IArticle> get_article(const int &id) = 0;

	virtual std::vector<std::shared_ptr<IArticle>> get_articles() = 0;

	virtual bool remove_article(const int &id) = 0;

protected:
	INewsgroup() { };
};

#endif
