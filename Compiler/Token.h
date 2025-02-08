#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "TokenType.h"

class Token {
public:
	TokenType type;
	std::string value;
};

#endif // !TOKEN_H
