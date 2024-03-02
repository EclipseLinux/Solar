#include "../global.h"
#include "commands.h"
#include "src/types.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>

inline std::vector<std::string> packages;

void installPkg(const std::string& pkg, PackageType type);

auto Commands::Install::_register(CLI::App& app) -> CLI::App*
{
	auto* sub = app.add_subcommand("install", "Install a package");

	sub->add_option("packages", packages, "Packages to install");

	return sub;
}

void Commands::Install::exec(const CLI::App& app)
{
	auto* pathVar = getenv("PATH");
	bool useGit = false;

	if (pathVar != nullptr)
	{
		auto splitPath = G::split(pathVar, ":");
		for (const auto& path : splitPath)
		{
			if (std::filesystem::exists(std::filesystem::path(path) / "git"))
			{
				useGit = true;
			}
		}
	}

	if (std::filesystem::exists("/usr/bin/git"))
	{
		useGit = true;
	}

	if (!useGit)
	{
		std::cout << rang::style::bold << rang::fgB::cyan << ":: " << rang::style::reset
				  << rang::fgB::yellow << "Git is not installed in your system, you "
				  << rang::style::bold
				  << "won't be able to install any Git-based packages"
				  << rang::style::reset << '\n';

		std::cout << rang::style::bold << rang::fgB::cyan << ":: " << rang::style::reset
				  << rang::fgB::cyan << "If you want to do so, run \""
				  << rang::style::bold << "solar install git" << rang::style::reset
				  << "\"\n";
	}

	for (auto& package : packages)
	{
		auto type = G::getPackageType(package);

		if (type == PackageType::GitRepo)
		{
			if (!useGit)
			{
				std::cerr << rang::style::bold << rang::fgB::cyan
						  << ":: " << rang::style::reset << rang::fgB::red
						  << "Git is not installed in your system, yet you "
						  << rang::style::bold << "tried to install a Git-based package"
						  << rang::style::reset << '\n';

				std::cout << rang::style::bold << rang::fgB::cyan
						  << ":: " << rang::style::reset << rang::fgB::cyan
						  << "If you want to do so, run \"" << rang::style::bold
						  << "solar install git" << rang::style::reset << "\"\n";

				exit(2);
			}
		}

		installPkg(package, type);
	}
}

void installPkg(const std::string& pkg, PackageType type)
{
	switch (type)
	{
	case PackageType::Default:
	case PackageType::GitRepo:
	case PackageType::RemoteFile:
	case PackageType::File:
	case PackageType::Debian:
	case PackageType::Rpm:
	case PackageType::AppImage:
	case PackageType::FlatPak:
	case PackageType::Snap:
	default:
		break;
	}
}