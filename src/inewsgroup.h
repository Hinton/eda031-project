#ifndef INEWSGROUP_H
#define INEWSGROUP_H

#include <string>

class INewsgroup
{
public:
  virtual ~INewsgroup() {};

  virtual int get_id();
  //virtual void setId(const int &id);

  virtual string get_title();
  virtual void set_title(const std::string &title);

protected:
  INewsgroup() {};
};

#endif