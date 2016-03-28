//
// Created by Samuel Johansson on 26/03/16.
//

#include <ctime>
#include "inmemoryarticle.h"

InMemoryArticle::InMemoryArticle(const int &id, const std::string title, const std::string author,
								 const std::string text) :
		timestamp(std::time(0)),
		id(id),
		title(title),
		author(author),
		text(text) { }

