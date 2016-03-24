#ifndef IDATABASE_H
#define IDATABASE_H

#include <vector>
#include "inewsgroup.h"
#include "iarticle.h"

class IDatabase
{
public:
  // ~IDatabase = 0; funkar ej, se 
  // http://www.geeksforgeeks.org/pure-virtual-destructor-c/
  virtual ~IDatabase() {};

  virtual std::vector<INewsgroup*> getNewsgroups();
  virtual INewsgroup* getNewsgroup(const int& id);
  virtual INewsgroup* addNewsgroup(const int& id, const std::string &title);

  virtual std::vector<IArticle*> getArticles(const int& newsgroup_id);
  virtual IArticle* getArticle(const int& id);
  virtual IArticle* addArticle(const int& id, const std::string &title, 
    const std::string &author, const std::String &text);

protected:
  IDatabase() {};
};

#endif