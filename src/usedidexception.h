//
// Created by Samuel Johansson on 28/03/16.
//

#include <stdexcept>

#ifndef EDA031_PROJECT_USEDID_H
#define EDA031_PROJECT_USEDID_H

struct UsedIDException : public std::runtime_error {
	explicit UsedIDException(const std::string &s) : std::runtime_error(s) { }
};

#endif
