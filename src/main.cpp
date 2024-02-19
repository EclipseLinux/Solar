#include "CLI/CLI.hpp"
#include "global.h"
#include "rang.hpp"
#include "src/commands/commands.h"
#include <iostream>

auto main(int argc, const char** argv) -> int
{
	CLI::App app{"The package manager for Eclipse", "Solar"};

	app.add_flag("-v,--verbose", G::verbose, "Enable verbose output");
	app.add_flag("-r,--root", G::root, "Simulated root to use (default /)");
	app.add_flag("-y,--yes", G::yes, "Say yes to every confirmation prompt");
	app.add_flag("-n,--nohook", G::noHook, "Disable hooks");

	app.require_subcommand();

	auto* install = Commands::Install::_register(app);
	auto* info = Commands::Info::_register(app);
	auto* init = Commands::Init::_register(app);

	std::atexit([]() { std::cout << rang::style::reset; });
	try
	{
		app.parse(argc, argv);
	}
	catch (const CLI::ParseError& e)
	{
		std::cout << (e.get_exit_code() == 0 ? rang::fg::blue : rang::fg::red);
		return app.exit(e);
	}

	if (app.got_subcommand(install))
	{
		Commands::Install::exec(app);
	}
	else if (app.got_subcommand(info))
	{
		Commands::Info::exec(app);
	}
	else if (app.got_subcommand(init))
	{
		Commands::Init::exec(app);
	}

	return 0;
}