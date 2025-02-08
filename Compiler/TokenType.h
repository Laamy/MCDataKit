#include <map>
#ifndef TOKENTYPE_H
#define TOKENTYPE_H

enum class TokenType {
	Identifier, Keyword, Number, String, Symbol, Operator, Boolean, Nil, EndOfFile
};

//debug
std::map<TokenType, std::string> tokenTypeToString = {
	{TokenType::Identifier, "Identifier"},
	{TokenType::Keyword, "Keyword"},
	{TokenType::Number, "Number"},
	{TokenType::String, "String"},
	{TokenType::Symbol, "Symbol"},
	{TokenType::Operator, "Operator"},
	{TokenType::Boolean, "Boolean"},
	{TokenType::Nil, "Nil"},
	{TokenType::EndOfFile, "EndOfFile"}
};

#endif // !TOKENTYPE_H
