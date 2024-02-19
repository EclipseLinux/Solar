#include "../global.h"
#include "commands.h"
#include <filesystem>
#include <vector>

inline std::vector<std::string> packages;
void installFile(const std::string& file);

auto Commands::Install::_register(CLI::App& app) -> CLI::App*
{
	auto* sub = app.add_subcommand("install", "Install a package");

	sub->add_option("packages", packages, "Packages to install");

	return sub;
}

void Commands::Install::exec(CLI::App& app)
{
	for (const auto& package : packages)
	{
		if (std::filesystem::exists(package))
		{
			installFile(package);
		}
	}
}

void installFile(const std::string& file)
{
	auto extension = std::filesystem::path(file).extension();

	if (extension == ".deb")
	{
	}
}