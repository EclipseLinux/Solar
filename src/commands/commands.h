#pragma once
#include "CLI/CLI.hpp"

namespace Commands
{
	namespace Install
	{
		auto _register(CLI::App& app) -> CLI::App;
		void exec(CLI::App& app);
	}

	namespace Info
	{
		auto _register(CLI::App& app) -> CLI::App;
		void exec(CLI::App& app);
	}
}