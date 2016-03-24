#ifndef IDATABASE_H
#define IDATABASE_H

#include <vector>
#include <memory>
#include "inewsgroup.h"
#include "iarticle.h"

class IDatabase
{
public:
  // ~IDatabase = 0; funkar ej, se 
  // http://www.geeksforgeeks.org/pure-virtual-destructor-c/
  virtual ~IDatabase() {};

  virtual std::vector<std::shared_ptr<INewsgroup>> getNewsgroups();
  virtual std::shared_ptr<INewsgroup> getNewsgroup(const int& id);
  virtual std::shared_ptr<INewsgroup> addNewsgroup(const int& id, const std::string &title);

  virtual std::vector<std::shared_ptr<IArticle>> getArticles(const int& newsgroup_id);
  virtual std::shared_ptr<IArticle> getArticle(const int& id);
  virtual std::shared_ptr<IArticle> addArticle(const int& id, const std::string &title, 
    const std::string &author, const std::String &text);

protected:
  IDatabase() {};
};

#endif