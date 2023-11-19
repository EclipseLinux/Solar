#pragma once
#include <filesystem>
#include <fstream>
#include <istream>
#include <string>
#include <vector>

namespace G
{
	inline bool verbose;
	inline std::string root = "/";
	inline bool yes;
	inline bool noHook;

	inline auto split(const std::string& str, const std::string& delimiter)
		-> std::vector<std::string>
	{
		size_t pos_start = 0;
		size_t pos_end;
		size_t delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos)
		{
			token = str.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(str.substr(pos_start));
		return res;
	}

	inline auto parseMirrors() -> std::vector<std::string>
	{
		auto file =
			std::ifstream(std::filesystem::path(root) / "etc" / "solar" / "mirrorlist");

		std::stringstream str;
		str << file.rdbuf();

		return split(str.str(), "\n");
	}

	inline auto parseInstalled() -> std::vector<std::string>
	{
		auto file =
			std::ifstream(std::filesystem::path(root) / "etc" / "solar" / "installed");

		std::stringstream str;
		str << file.rdbuf();

		return split(str.str(), " ");
	}

	inline auto parseRemoved() -> std::vector<std::string>
	{
		auto file =
			std::ifstream(std::filesystem::path(root) / "etc" / "solar" / "removed");

		std::stringstream str;
		str << file.rdbuf();

		return split(str.str(), "\n");
	}

	inline auto parseUpdated() -> std::vector<std::string>
	{
		auto file =
			std::ifstream(std::filesystem::path(root) / "etc" / "solar" / "updated");

		std::stringstream str;
		str << file.rdbuf();

		return split(str.str(), "\n");
	}
}