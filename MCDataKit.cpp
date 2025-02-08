#include <iostream>
#include <sstream>
#include <string>

#include "Compiler/LuaTokenizer.h"
#include "Compiler/MCDatapack.h"

#include "Envrionment/Envrionment.h"

int main()
{
	LuaTokenizer tokenizer(R"(

# this executes on load.json & datapack:load
say("Loaded datapack")

function test()	
	if cache.test >= 2 then

		if cache.test <= 5 then
			say("cache is within 2-5")
			gamemode(Survival)
		end
	end
end

)");

	auto tokens = tokenizer.tokenize();

	//for (const auto& token : tokens)
	//	std::cout << "type " << tokenTypeToString[token.type] << ", value " << token.value << std::endl;

	MCDatapack pack(tokens);

	pack.RegisterFunction("say", Envrionment::env_say);
	pack.RegisterFunction("gamemode", Envrionment::env_gamemode);

	try {
		auto datapack = pack.Rebuild();

		for (auto func : datapack) {
			std::cout << "\nFunc Name: " << func.FuncName << " Content:\n" << func.Content << "\n";
		}
	}
	catch (std::exception ex) {
		std::cout << "Error while compiling lua2datapack " << ex.what();
	}
}
