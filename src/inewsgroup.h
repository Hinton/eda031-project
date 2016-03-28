#ifndef INEWSGROUP_H
#define INEWSGROUP_H

#include <string>
#include <vector>
#include <memory>

class IArticle;
class INewsgroup {
public:
	virtual ~INewsgroup() { };

	virtual int get_id();
	//virtual void setId(const int &id);

	virtual std::string get_title();

	virtual void set_title(const std::string &title);

	virtual std::shared_ptr<IArticle> add_article(const int &id, const std::string &title,
												  const std::string &author, const std::string &text);

	virtual std::shared_ptr<IArticle> get_article(const int &id);

	virtual std::vector<std::shared_ptr<IArticle>> get_articles();

	virtual bool remove_article(const int &id);

protected:
	INewsgroup() { };
};

#endif
