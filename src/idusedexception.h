#ifndef IDUSEDEXCEPTION_H
#define IDUSEDEXCEPTION_H

#include <stdexcept>

struct IDUsedException : public std::runtime_error {
	explicit IDUsedException(const std::string &s) : std::runtime_error(s) { }
};

#endif
