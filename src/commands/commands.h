#pragma once
#include "CLI/App.hpp"

namespace Commands
{
	namespace Install
	{
		auto _register(CLI::App& app) -> CLI::App*;
		void exec(const CLI::App& app);
	}

	namespace Info
	{
		auto _register(CLI::App& app) -> CLI::App*;
		void exec(CLI::App& app);
	}

	namespace Init
	{
		auto _register(CLI::App& app) -> CLI::App*;
		void exec(CLI::App& app);
	}
}