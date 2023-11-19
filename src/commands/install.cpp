#include "../global.h"
#include "commands.h"

auto Commands::Install::_register(CLI::App& app) -> CLI::App*
{
	auto* sub = app.add_subcommand("install", "Install a package");

	return sub;
}

void Commands::Install::exec(CLI::App& app)
{
}