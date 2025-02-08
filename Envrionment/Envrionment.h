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
			throw new std::exception("No arguments provided");

		std::ostringstream oss;

		oss << "say ";
		for (const auto& arg : args)
			oss << arg.value << " ";

		context->AppendLine(oss.str().c_str());
	}
}

#endif // !ENVRIONMENT_H
