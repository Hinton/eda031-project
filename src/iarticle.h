#ifndef IARTICLE_H
#define IARTICLE_H

#include <string>

class IArticle
{
public:
  virtual ~IArticle() {};

  virtual int getId();
  //virtual void setId(const int &id);

  virtual string getTitle();
  virtual void setTitle(const std::string &title);

  virtual string getAuthor();
  virtual void setAuthor(const std::string &author);

  virtual string getText();
  virtual void setText(const std::string &text);

  virtual void remove();

protected:
  IArticle() {};
};

#endif