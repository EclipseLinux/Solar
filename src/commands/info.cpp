#include "../global.h"
#include "commands.h"
#include <iostream>

auto Commands::Info::_register(CLI::App& app) -> CLI::App*
{
	auto* sub = app.add_subcommand("info", "Print detailed info about Solar");

	return sub;
}

void Commands::Info::exec(CLI::App& app)
{
	std::cout << "Hello";
}