#include <string>
#ifndef MCFUNCTION_H
#define MCFUNCTION_H

enum MCFunctionEvent { WORLD_FUNC, WORLD_LOAD, WORLD_TICK };

class MCFunction {
public:
	MCFunctionEvent Event = MCFunctionEvent::WORLD_FUNC;

	std::string FuncName;
	std::string Content = "";

	void Append(std::string data) { Content += data; }
	void AppendLine(std::string data) { Content += data + "\n"; }
};

#endif // !MCFUNCTION_H
