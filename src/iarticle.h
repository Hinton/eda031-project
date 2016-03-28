#ifndef IARTICLE_H
#define IARTICLE_H

#include <string>

class IArticle
{
public:
	virtual ~IArticle() {};

	virtual int get_id() = 0;

	virtual std::string get_title() = 0;
	virtual void set_title(const std::string &title) = 0;

	virtual std::string get_author() = 0;
	virtual void set_author(const std::string &author) = 0;

	virtual std::string get_text() = 0;
	virtual void set_text(const std::string &text) = 0;

protected:
	IArticle() {};
};

#endif
