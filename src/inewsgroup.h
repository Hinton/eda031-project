#ifndef INEWSGROUP_H
#define INEWSGROUP_H

#include <string>

class INewsgroup
{
public:
  virtual ~INewsgroup() {};

  virtual int getId();
  //virtual void setId(const int &id);

  virtual string getTitle();
  virtual void setTitle(const std::string &title);

  virtual void remove();

protected:
  INewsgroup() {};
};

#endif