#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include <exception>

class obj_already_exists : public std::exception 
{
public:
	virtual const char* what() const throw()
	{
		return "The object cannot be created because it already exists.";
	}
};

class obj_not_found : public std::exception
{
public:
	obj_not_found(const string &msg) : msg(msg) {};
	virtual const char* what() const throw()
	{
		if (msg.empty())
			return "The object you are searching for could not be found.";
		else 
			return msg;
	}
private:
	string msg;
};

class func_not_supported : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "The function you are trying to use is not supported at the moment.";
	}
};

class func_not_implemented : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "The function you are trying to use is not yet implemented. Work in progress.";
	}
};

#endif
