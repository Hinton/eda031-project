#ifndef INMEMORYARTICLE_H
#define INMEMORYARTICLE_H

#include <string>
#include "iarticle.h"

class InMemoryArticle : public IArticle {
public:
	InMemoryArticle(const int &id, const std::string title, const std::string author, const std::string text);

	int get_id() { return id; }
	//virtual void setId(const int &id);

	std::string get_title() { return title; }

	void set_title(const std::string &title) { this->title = title; }

	std::string get_author() { return author; }

	void set_author(const std::string &author) { this->author = author; }

	std::string get_text() { return text; }

	void set_text(const std::string &text) { this->text = text; }

private:
	const int id;
	std::string title, author, text;
};

#endif
