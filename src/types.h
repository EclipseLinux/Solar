#pragma once
#include <string>
#include <vector>

enum class PackageType
{
	Default,
	GitRepo,
	RemoteFile,
	File,
	Debian,
	Rpm,
	AppImage,
	FlatPak,
	Snap,
	Brotli,

	//+-------------------------------------+
	//|Other Package Types (pip, make, etc) |
	//+-------------------------------------+

	Registry,
	CMake,
	Meson,
	Make,
	Pip,
	Npm,
	Yarn,
	Cargo,
	Go,
	Custom
};

struct PackageInfo
{
public:
	std::string Name;
	std::string Version;
	std::string Description;
	std::string GitRepoURL;
	std::string SiteURL;
	std::string License;
	std::string Creator;
	std::string BugURL;
	std::vector<std::string> Contributors;
};

struct BasePackage
{
public:
	std::string Name;
	std::string Version;
	std::string Source;
	PackageType Type;

	std::vector<std::string> Dependencies;
	std::vector<std::string> Breaks;
	std::vector<std::string> Conflicts;
	std::vector<std::string> Replaces;

	bool Explicit;
};

struct Package : public BasePackage
{
public:
	std::string Download;
	std::string Checksum;
	unsigned long Compressed;
	unsigned long Uncompressed;
	std::string Publisher;
	PackageInfo Info;

	bool PreRelease;
};