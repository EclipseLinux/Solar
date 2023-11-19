#include "../global.h"
#include "commands.h"
#include "src/rang.hpp"
#include <cstdlib>
#include <iostream>
#include <ostream>

auto Commands::Init::_register(CLI::App& app) -> CLI::App*
{
	auto* sub = app.add_subcommand(
		"init", "Create the necessary files for Solar to work on an empty environment");

	return sub;
}

void Commands::Init::exec(CLI::App& app)
{
	auto path = std::filesystem::path(G::root) / "etc" / "solar";

	if (!std::filesystem::exists(path) && std::filesystem::create_directories(path))
	{
		std::cout << rang::style::bold << rang::fgB::green << "✓ " << rang::style::reset
				  << rang::fgB::green << "Created " << rang::style::bold
				  << "Solar directory" << rang::style::reset << std::endl;
	}
	else
	{
		std::cout << rang::style::bold << rang::fgB::red << "𐄂 " << rang::style::reset
				  << rang::fgB::red << "Failed to create " << rang::style::bold
				  << "Solar directory (/etc/solar)" << rang::style::reset << std::endl;
	}

	std::ofstream mirrorlist;
	mirrorlist.open(path / "mirrorlist");
	mirrorlist << "https://raw.githubusercontent.com/EclipseLinux/SolarRegistry/main";
	mirrorlist.close();

	std::cout << rang::style::bold << rang::fgB::green << "✓" << rang::style::reset
			  << rang::fgB::green << "Created " << rang::style::bold << "mirrorlist"
			  << rang::style::reset << std::endl;

	std::ofstream(path / "installed").close();
	std::ofstream(path / "removed").close();
	std::ofstream(path / "updated").close();
}