#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include <exception>

class obj_already_exists : public exception
{
  virtual const char* what() const throw()
  {
    return "The object cannot be created because it already exists.";
  }
};

class obj_not_found : public exception
{
  virtual const char* what() const throw()
  {
    return "The object you are searching for could not be found.";
  }
};

#endif