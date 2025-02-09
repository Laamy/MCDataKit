#ifndef MCDATAPACK_H
#define MCDATAPACK_H

#include <vector>
#include <unordered_map>
#include <functional>
#include <stack>

#include "Token.h"
#include "MCFunction.h"
#include "../DequeStringStream.h"

class MCDatapack {
private:
	std::vector<Token> tokens;
	std::unordered_map<std::string, std::function<void(MCFunction*, const std::vector<Token>&)>> functions;

	// misc
	std::stack<MCFunction> protos{};

	// compile info
	std::vector<MCFunction> datapackFuncs;
	std::string datapackName = "datapack";
	std::string datapackDesc = "Description";
	std::string datapackAuthor = "Author";

	__forceinline void throw_raw_error(std::string err) {

		throw std::runtime_error(err);
	}

	__forceinline void throw_error(Token token, const char* errorInfo) {
		std::ostringstream oss;
		oss << ". " << errorInfo << " Type '" << tokenTypeToString[token.type];
		oss << "' Token value: '" << token.value << "'";

		throw std::runtime_error(oss.str());
	}

	__forceinline std::string random(std::string prefix) {
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		std::string tmp_s;
		tmp_s.reserve(5);

		for (int i = 0; i < 5; i++)
			tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

		return prefix + tmp_s;
	}

public:
	explicit MCDatapack(const std::vector<Token>& tokens) : tokens(tokens) {}

	void RegisterFunction(
		const std::string& name,
		std::function<void(MCFunction*, const std::vector<Token>&)> function) {
		functions[name] = function;
	}

	std::vector<MCFunction> Rebuild() {

		// our inital envrionmeent
		protos.push(MCFunction("load"));
		protos.top().Event = MCFunctionEvent::WORLD_LOAD;

		for (int i = 0; i < tokens.size(); i++)
		{
			auto thing = HandleToken(i, tokens[i]);
			if (!thing.empty())
				return thing;
		}
	}

	std::vector<MCFunction> HandleToken(int& i, Token token) {
		Token next = (token.type == TokenType::EndOfFile ? Token{ TokenType::EndOfFile, "" } : tokens[i + 1]);

		std::cout << "type " << tokenTypeToString[token.type] << ", value " << token.value << std::endl;

		// inbuilt function use case
		if (token.type == TokenType::Identifier) {
			std::vector<Token> arguments;
			if (next.type == TokenType::Symbol && (i + 2) < tokens.size() && tokens[i + 2].type != TokenType::EndOfFile) {
				i += 2;
				while (tokens[i].value != "\)") {
					if (i + 1 >= tokens.size()) throw_raw_error("Unable to finish script due to EndOfFile");

					arguments.push_back(tokens[i]);
					i++;
				}

				auto functionInfo = functions.find(token.value);
				if (functionInfo != functions.end())
					// caller, ...arguments
					functionInfo->second(&protos.top(), arguments);

				return {};
			}
			else throw_error(token, "Unexpected");
		}
		else if (token.type == TokenType::Keyword) {
			if (token.value == "function") {
				protos.push(MCFunction(""));

				if (next.type == TokenType::Identifier)
				{
					protos.top().SetName(next.value);

					i++;
					next = (token.type == TokenType::EndOfFile ? Token{ TokenType::EndOfFile, "" } : tokens[i + 1]);

					if (next.type == TokenType::Symbol && next.value == "(") {
						i++;
						next = (token.type == TokenType::EndOfFile ? Token{ TokenType::EndOfFile, "" } : tokens[i + 1]);

						if (next.type == TokenType::Symbol && next.value == ")") { // no handlling of function arguments yet..
							i++;
							next = (token.type == TokenType::EndOfFile ? Token{ TokenType::EndOfFile, "" } : tokens[i + 1]);
						}
					}
				}
			}
			// temp hardcoded solution to get a concept up and running
			else if (token.value == "if") { // appends execute if
				token = tokens[i += 1];

				if (token.type == TokenType::Identifier && token.value == "cache") {
					std::string boardName = token.value;
					DequStringStream builder;
					builder.set("execute if ");
					// /execute if score $(blank) cache

					token = tokens[i += 1];

					if (token.type == TokenType::Symbol && token.value == ".") {
						token = tokens[i += 1];

						if (token.type == TokenType::Identifier) {
							builder.front(std::string(token.value) + " ");
							builder.front(std::string(boardName) + " ");
							// /execute if score test cache
							token = tokens[i += 1];

							if (token.type == TokenType::Operator) {
								builder.front(token.value + " ");
								// /execute if score test cache >=
								token = tokens[i += 1];

								if (token.type == TokenType::Number) {
									// /scoreboard players add temp_8D2k3 cache 2
									// /execute if score test cache >= temp_8D2k3 cache

									std::string tempVar = random("temp_"); // temp scoreboard entry to allow for comparing to hardcoded numbers
									builder.back("scoreboard players add " + tempVar + " " + boardName + " " + token.value + "\n");
									builder.front(tempVar + " " + boardName + " ");
									token = tokens[i += 1];

									if (token.type == TokenType::Keyword) {
										if (token.value == "then") {
											// /scoreboard players add temp_8D2k3 cache 2
											// /execute if score test cache >= temp_8D2k3 cache run datapack:internal/if_o2D03
											// /scoreboard players remove temp_8D2k3 cache
											std::string hardName = random("internal/if_");
											builder.front("run " + datapackName + ":" + hardName);
											builder.front("\nscoreboard players remove " + tempVar + " " + boardName);
											protos.top().AppendLine(builder.str());

											protos.push(MCFunction(hardName));
										}
										else throw_error(token, "Expected <then> got");
									}
									else throw_error(token, "Expected Keyword got");
								}
								else throw_error(token, "Expected 'Number' got");
							}
							else throw_error(token, "Expected Operator got");
						}
						else throw_error(token, "Expected Identifier got");
					}
					else throw_error(token, "Expected Symbol");
				}
				else throw_error(token, "Expected Identifier got");
			}
			else if (token.value == "end") {
				datapackFuncs.push_back(protos.top());
				protos.pop();
			}
			else throw_error(token, "Unexpected");
		}
		else if (token.type == TokenType::EndOfFile) {
			// finally push the load function..
			datapackFuncs.push_back(protos.top());
			protos.pop();

			return datapackFuncs;
		}
		else throw_error(token, "Unhandled token");

		return {};
	}
};

#endif // !MCDATAPACK_H
