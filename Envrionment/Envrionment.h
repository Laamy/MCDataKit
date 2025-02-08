#ifndef ENVRIONMENT_H
#define ENVRIONMENT_H

#include <string>
#include <sstream>
#include <vector>

#include "../Compiler/Token.h"
#include "../Compiler/MCFunction.h"

namespace Envrionment {
	std::string VERSION = "v0.1-dev";

	void env_say(MCFunction* context, const std::vector<Token>& args) {
		if (args.empty())
			throw std::runtime_error("No arguments provided");

		std::string message = "say ";

		for (const auto& arg : args)
			message += arg.value + " ";

		context->AppendLine(message);
	}

	void env_gamemode(MCFunction* context, const std::vector<Token>& args) {
		static std::vector<std::string> gamemodes = { "creative", "survival", "adventure", "spectator" };

		std::string data = args[0].value;
		for (auto& c : data)
			c = std::tolower(c);

		if (args.empty() || args.size() > 1)
			throw std::runtime_error("Invalid amount of arguments provided, expected string.");

		if (args[0].type != TokenType::String && args[0].type != TokenType::Identifier)
			throw std::runtime_error("Invalid argument type '" + tokenTypeToString[args[0].type] + "'. expected '<String,Identifier>'");

		if (std::find(gamemodes.begin(), gamemodes.end(), data) == gamemodes.end())
			throw std::runtime_error("Invalid gamemode '" + args[0].value + "'. expected type of <creative,survival,adventure,spectator>");

		context->AppendLine("gamemode " + data);
	}
}

#endif // !ENVRIONMENT_H
