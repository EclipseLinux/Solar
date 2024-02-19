#include "../global.h"
#include "commands.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "src/rang.hpp"
#include <fstream>

auto Commands::Init::_register(CLI::App& app) -> CLI::App*
{
	auto* sub = app.add_subcommand(
		"init", "Create the necessary files for Solar to work on an empty environment");

	return sub;
}

void Commands::Init::exec(CLI::App& app)
{
	auto path = std::filesystem::path(G::root) / "etc" / "solar";

	rapidjson::StringBuffer lock;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> lockWriter(lock);

	lockWriter.StartObject();
	lockWriter.Key("packages");
	lockWriter.StartArray();
	lockWriter.EndArray();
	lockWriter.EndObject();

	rapidjson::StringBuffer config;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> configWriter(lock);

	configWriter.StartObject();

	configWriter.Key("color");
	configWriter.String("auto");

	configWriter.Key("fancyBar");
	configWriter.String("auto");

	configWriter.Key("rootDir");
	configWriter.String("/");

	configWriter.Key("arch");
	configWriter.String("auto");

	configWriter.Key("progress");
	configWriter.Bool(true);

	configWriter.Key("checkSpace");
	configWriter.Bool(true);

	configWriter.Key("parallelDownloads");
	configWriter.String("auto");

	configWriter.EndObject();

	std::ofstream lockFile(path / "solar.lock.json");
	lockFile << lock.GetString();
	lockFile.close();

	std::ofstream configFile(path / "config.json");
	configFile << config.GetString();
	configFile.close();

	std::cout << rang::style::bold << rang::fgB::green
			  << "✅ Created lock and config files!" << rang::style::reset << '\n';
}