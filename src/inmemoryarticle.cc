#include <ctime>
#include "inmemoryarticle.h"

InMemoryArticle::InMemoryArticle(const int &id, const std::string title, const std::string author,
								 const std::string text) :
		id(id),
		title(title),
		author(author),
		text(text) { }

