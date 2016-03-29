#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include <exception>
#include <string>

class obj_already_exists : public std::exception 
{
public:
	virtual const char* what() const throw()
	{
		return "The object cannot be created because it already exists.";
	}
};

class group_already_exists : public std::exception {
public:
	virtual const char *what() const throw() {
		return "The newsgroup cannot be created because it already exists.";
	}
};

class group_not_found : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "The newsgroup you are searching for could not be found.";
	}
};

class article_not_found : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "The article you are searching for could not be found.";
	}
};

class obj_not_found : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "[DEPRECATED; DONT USE] The object you are searching for could not be found.";
	}
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
