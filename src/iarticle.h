#ifndef IARTICLE_H
#define IARTICLE_H

#include <string>

class IArticle
{
public:
  virtual ~IArticle() {};

  virtual int get_id();
  //virtual void setId(const int &id);

  virtual string get_title();
  virtual void set_title(const std::string &title);

  virtual string get_author();
  virtual void set_author(const std::string &author);

  virtual string get_text();
  virtual void set_text(const std::string &text);

protected:
  IArticle() {};
};

#endif