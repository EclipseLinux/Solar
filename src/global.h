#pragma once
#include <cassert>
#include <regex>
#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#include "curl/curl.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "src/rang.hpp"
#include "types.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace G
{
	inline bool verbose;
	inline std::string root = "/";
	inline bool yes;
	inline bool noHook;

	inline auto writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data)
		-> size_t
	{
		data->append((char*)ptr, size * nmemb);
		return size * nmemb;
	}

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

	static auto EndsWith(std::string_view str, std::string_view suffix) -> bool
	{
		return str.size() >= suffix.size() &&
			   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}

	static auto StartsWith(std::string_view str, std::string_view prefix) -> bool
	{
		return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
	}

	inline auto getPackageType(std::string& pkg) -> PackageType
	{
		if (StartsWith(pkg, "@"))
		{
			pkg.erase(0, 1);
			auto url = split(pkg, "/");

			if (url.size() != 2)
			{
				std::cerr << rang::style::bold << rang::fgB::cyan
						  << ":: " << rang::style::reset << rang::fgB::red
						  << "Invalid GitHub repo \"" << rang::style::bold << pkg
						  << rang::style::reset << rang::fgB::red << "\"\n"
						  << rang::style::reset;
				return PackageType::GitRepo;
			}

			pkg = "https://github.com/" + url[0] + "/" + url[1] + ".git";
			return PackageType::GitRepo;
		}

		if (std::regex_match(pkg, std::regex("([a-zA-Z-_0-9]+/)?[a-zA-Z-_0-9]+")))
		{
			auto pkgSplit = split(pkg, "/");

			if (pkgSplit.size() >= 3)
			{
				std::cerr << rang::style::bold << rang::fgB::cyan
						  << ":: " << rang::style::reset << rang::fgB::red
						  << "Invalid package \"" << rang::style::bold << pkg
						  << rang::style::reset << rang::fgB::red << "\"\n"
						  << rang::style::reset;
				return PackageType::Default;
			}

			pkg = pkgSplit.size() == 2 ? pkgSplit[1] : pkgSplit[0];

			return PackageType::Default;
		}

		if (EndsWith(pkg, ".git"))
		{
			return PackageType::GitRepo;
		}

		if (std::filesystem::exists(pkg))
		{
			if (EndsWith(pkg, ".solar.json"))
			{
				return PackageType::File;
			}

			if (EndsWith(pkg, ".deb"))
			{
				return PackageType::Debian;
			}

			if (EndsWith(pkg, ".rpm"))
			{
				return PackageType::Rpm;
			}

			if (EndsWith(pkg, ".AppImage"))
			{
				return PackageType::AppImage;
			}

			if (EndsWith(pkg, ".snap"))
			{
				return PackageType::Snap;
			}

			if (EndsWith(pkg, ".flatpak"))
			{
				return PackageType::FlatPak;
			}

			if (EndsWith(pkg, ".br"))
			{
				return PackageType::Brotli;
			}
		}

		if (EndsWith(pkg, ".solar.json"))
		{
			return PackageType::RemoteFile;
		}

		auto* curl = curl_easy_init();
		std::string responseBody;
		std::string header;

		long responseCode;

		if (curl != nullptr)
		{
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		}
		else
		{
			std::cerr << rang::style::bold << rang::fgB::cyan
					  << ":: " << rang::style::reset << rang::fgB::red
					  << "cURL failed to initialize" << rang::style::reset << '\n';
			exit(1);
		}

		curl_easy_setopt(curl, CURLOPT_URL, (pkg + ".git").c_str());
		curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		if (responseCode != 404)
		{
			pkg = pkg + ".git";
			return PackageType::GitRepo;
		}

		curl_easy_setopt(curl, CURLOPT_URL, (pkg + ".solar.json").c_str());
		curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		if (responseCode != 404)
		{
			pkg = pkg + ".solar.json";
			return PackageType::RemoteFile;
		}

		curl_easy_cleanup(curl);
		curl = nullptr;

		return PackageType::Default;
	}

	inline auto parsePackageType(const std::string& type) -> PackageType
	{
		if (type == "Registry")
		{
			return PackageType::Registry;
		}
		if (type == "pip")
		{
			return PackageType::Pip;
		}
		if (type == "cmake")
		{
			return PackageType::CMake;
		}
		if (type == "meson")
		{
			return PackageType::Meson;
		}
		if (type == "make")
		{
			return PackageType::Make;
		}
		if (type == "cargo")
		{
			return PackageType::Cargo;
		}
		if (type == "npm")
		{
			return PackageType::Npm;
		}
		if (type == "yarn")
		{
			return PackageType::Yarn;
		}
		if (type == "custom")
		{
			return PackageType::Custom;
		}
		if (type == "go")
		{
			return PackageType::Go;
		}

		return PackageType::Registry;
	}

	inline auto parseBasePackage(rapidjson::Document& doc) -> BasePackage
	{
		auto pkg = BasePackage();

		pkg.Name = doc["name"].GetString();
		pkg.Version = doc["version"].GetString();
		pkg.Type = parsePackageType(std::string(doc["type"].GetString()));

		if (doc.HasMember("explicit"))
		{
			pkg.Explicit = doc["explicit"].GetBool();
		}

		assert(doc.HasMember("dependencies"));

		for (auto* itr = doc["dependencies"].GetArray().Begin();
			 itr != doc["dependencies"].GetArray().End(); ++itr)
		{
			pkg.Dependencies.emplace_back(itr->GetString());
		}

		if (doc.HasMember("breaks"))
		{
			for (auto* itr = doc["breaks"].GetArray().Begin();
				 itr != doc["breaks"].GetArray().End(); ++itr)
			{
				pkg.Breaks.emplace_back(itr->GetString());
			}
		}

		if (doc.HasMember("conflicts"))
		{
			for (auto* itr = doc["conflicts"].GetArray().Begin();
				 itr != doc["conflicts"].GetArray().End(); ++itr)
			{
				pkg.Conflicts.emplace_back(itr->GetString());
			}
		}

		if (doc.HasMember("replaces"))
		{
			for (auto* itr = doc["replaces"].GetArray().Begin();
				 itr != doc["replaces"].GetArray().End(); ++itr)
			{
				pkg.Replaces.emplace_back(itr->GetString());
			}
		}

		return pkg;
	}

	inline auto parsePackageInfo(rapidjson::Document& json) -> PackageInfo
	{
		assert(json.HasMember("pkgInfo"));

		auto doc = json["pkgInfo"].GetObject();
		auto pkg = PackageInfo();

		pkg.Name = doc["name"].GetString();
		pkg.Version = doc["version"].GetString();
		pkg.Description = doc["description"].GetString();

		if (doc.HasMember("gitUrl"))
		{
			pkg.GitRepoURL = doc["gitUrl"].GetString();
		}

		if (doc.HasMember("siteUrl"))
		{
			pkg.SiteURL = doc["siteUrl"].GetString();
		}

		if (doc.HasMember("license"))
		{
			pkg.License = doc["license"].GetString();
		}

		if (doc.HasMember("bugUrl"))
		{
			pkg.BugURL = doc["bugUrl"].GetString();
		}

		if (doc.HasMember("creator"))
		{
			pkg.Creator = doc["creator"].GetString();
		}

		if (doc.HasMember("contributors"))
		{
			for (auto* itr = doc["contributors"].GetArray().Begin();
				 itr != doc["contributors"].GetArray().End(); ++itr)
			{
				pkg.Contributors.emplace_back(itr->GetString());
			}
		}

		return pkg;
	}

	inline auto parsePackage(const std::filesystem::path& path) -> Package
	{
		auto pkg = Package();
		auto file = std::ifstream(path);

		rapidjson::IStreamWrapper stream(file);

		rapidjson::Document doc;
		doc.ParseStream(stream);

		//+------------------+
		//|Parse BasePackage |
		//+------------------+
		auto basePkg = parseBasePackage(doc);
		pkg.Name = basePkg.Name;
		pkg.Version = basePkg.Version;
		pkg.Explicit = basePkg.Explicit;
		pkg.Dependencies = basePkg.Dependencies;
		pkg.Breaks = basePkg.Breaks;
		pkg.Replaces = basePkg.Replaces;
		pkg.Conflicts = basePkg.Conflicts;
		pkg.Type = basePkg.Type;

		//+-----------------+
		//|Parse Package	|
		//+-----------------+

		assert(doc.HasMember("type"));

		if (doc.HasMember("download"))
		{
			pkg.Download = doc["download"].GetString();
		}

		if (doc.HasMember("checksum") && pkg.Type == PackageType::Registry)
		{
			pkg.Download = doc["checksum"].GetString();
		}
		else
		{
			std::cout << rang::style::bold << rang::fgB::cyan
					  << ":: " << rang::style::reset << rang::fgB::yellow << "Package \""
					  << rang::style::bold << pkg.Name << rang::style::reset
					  << rang::fgB::yellow << "\" does " << rang::style::bold << "not"
					  << rang::style::reset << rang::fgB::yellow
					  << " have a checksum, this might lead to vulnerability issues!"
					  << rang::style::reset << '\n';
		}

		if (doc.HasMember("compressed"))
		{
			pkg.Compressed = doc["compressed"].GetUint();
		}

		if (doc.HasMember("uncompressed"))
		{
			pkg.Uncompressed = doc["uncompressed"].GetUint();
		}

		if (doc.HasMember("publisher"))
		{
			pkg.Publisher = doc["publisher"].GetString();
		}

		if (doc.HasMember("prerelease"))
		{
			pkg.PreRelease = doc["prerelease"].GetBool();
		}

		pkg.Info = parsePackageInfo(doc);

		return pkg;
	}
}
