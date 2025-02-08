#ifndef LUATOKENIZER_H
#define LUATOKENIZER_H

#include <string>
#include <unordered_set>

#include "Token.h"

class LuaTokenizer {
private:
	std::string input;
	size_t pos = 0;

	const std::unordered_set<std::string> keywords = {
		"if", "then", "end", "function", "local"
	};

	const std::unordered_set<std::string> operators = {
		">=", "<=", ">", "<", "=", // execute if score
		"==" // set
	};

	char peek() { return pos < input.size() ? input[pos] : '\0'; }
	char get() { return pos < input.size() ? input[pos++] : '\0'; }

	// 
	void skipWhitespace() {
		while (std::isspace(peek()))
			get();
	}

	// TODO: --[[]] large/multiline comments
	void skipComment() {
		if (peek() == '#') {
			get();  // consume both -'s
			while (peek() != '\n' && peek() != '\0')
				get();
		}

		if (peek() == '-' && pos + 1 < input.size() && input[pos + 1] == '-') {
			get(); get(); // consume both -'s
			while (peek() != '\n' && peek() != '\0')
				get();
		}
	}

	Token parseNumber()
	{
		std::string num;
		while (std::isdigit(peek()) || peek() == '.') num += get();
		return { TokenType::Number, num };
	}

	Token parseString() {
		char quote = get();
		std::string str;
		while (peek() != quote && peek() != '\0') {
			str += get();
		}
		get(); // final quote
		return { TokenType::String, str };
	}

	Token parseIdentifier() {
		std::string id;
		while (std::isalnum(peek()) || peek() == '_') id += get();
		if (keywords.count(id)) return { TokenType::Keyword, id };

		if (id == "true" || id == "false") return { TokenType::Boolean, id };
		if (id == "nil") return { TokenType::Nil, id };

		return { TokenType::Identifier, id };
	}

	Token parseOperator() {
		std::string op(1, get());
		if (peek() && operators.count(op + peek())) op += get();
		return { TokenType::Operator, op };
	}

public:
	explicit LuaTokenizer(const std::string& src) : input(src) {}

	std::vector<Token> tokenize() {
		std::vector<Token> tokens;
		while (pos < input.size()) {
			skipWhitespace();
			skipComment();
			if (peek() == '\'' || peek() == '\"')tokens.push_back(parseString());
			else if (std::isdigit(peek())) tokens.push_back(parseNumber());
			else if (std::isalpha(peek()) || peek() == '_') tokens.push_back(parseIdentifier());
			else if (operators.count(std::string(1, peek()))) tokens.push_back(parseOperator());
			else if (peek() != '\0' && !std::isspace(peek())) tokens.push_back({ TokenType::Symbol, std::string(1, get()) });
		}
		tokens.push_back({ TokenType::EndOfFile, "" });
		return tokens;
	}
};

#endif // !LUATOKENIZER_H
