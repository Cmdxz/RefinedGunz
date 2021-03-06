#include "stdafx.h"
#include "MBMatchServer.h"
#include "MMatchConfig.h"

static std::string Line;
static std::vector<std::string> Splits;
// Doesn't count the command.
static int NumArguments;

static constexpr int NoArgumentLimit = -1;

void MBMatchServer::InitConsoleCommands()
{
	auto AddConsoleCommand = [&](const char* Name,
		int MinArgs, int MaxArgs,
		std::string Description, ::string Usage, std::string Help,
		auto&& Callback)
	{
		ConsoleCommand Command;
		Command.MinArgs = MinArgs;
		Command.MaxArgs = MaxArgs;
		Command.Description = std::move(Description);
		Command.Usage = std::move(Usage);
		Command.Help = std::move(Help);
		Command.Callback = std::move(Callback);
		ConsoleCommandMap.emplace(Name, std::move(Command));
	};

	AddConsoleCommand("hello",
		NoArgumentLimit, NoArgumentLimit,
		"Says hi.", "hello", "",
		[]{ MLog("Hi! ^__^\n"); });

	AddConsoleCommand("argv",
		NoArgumentLimit, NoArgumentLimit,
		"Prints all input.", "argv [Arg1 [Arg2 ... [ArgN]]]", "",
		[] {
		MLog("Line = %s\n", Line.c_str());
		MLog("NumArguments = %d\n", NumArguments);
		for (int i = 0; i < int(Splits.size()); ++i)
			MLog("Splits[%d] = \"%s\"\n", i, Splits[i].c_str());
	});

	AddConsoleCommand("help",
		NoArgumentLimit, 1,
		"Provides information about commands.",
		"help [command name]",
		"",
		[&] {
		if (NumArguments == 0)
		{
			// Print all the commands.
			for (auto&& Pair : ConsoleCommandMap)
			{
				MLog("%s: %s\n", Pair.first.c_str(), Pair.second.Description.c_str());
			}
		}
		else
		{
			auto it = ConsoleCommandMap.find(Splits[1]);
			if (it == ConsoleCommandMap.end())
			{
				MLog("help: Unknown command \"%s\"\n", Splits[1].c_str());
				return;
			}

			MLog("help: %s\n"
				"Description: %s\n"
				"Usage: %s\n"
				"Help: %s\n",
				it->first.c_str(),
				it->second.Description.c_str(),
				it->second.Usage.c_str(),
				it->second.Help.c_str());
		}
	});

	AddConsoleCommand("setversion", 2, 2,
		"Sets the expected client version.",
		"setversion <major/minor/patch/revision> <version number>",
		"Sets the specified field of MMatchConfig::Version to the specified value.\n"
		"This value is used when players log in, to check if their clients are up to date.\n"
		"Revision number is in hexadecimal.",
		[] {
		auto VersionTypeString = Splits[1].c_str();
		auto VersionValueString = Splits[2].c_str();

		auto SetVersion = [&](auto&& Version, int Radix) {
			const u32 NewVersion = strtoul(VersionValueString, nullptr, Radix);
			const auto OldVersion = Version;
			Version = NewVersion;
			MLog("Set %s version to %d! Previous %s version was %d.\n",
				VersionTypeString, NewVersion,
				VersionTypeString, OldVersion);
			return;
		};

		if (!_stricmp(VersionTypeString, "revision"))
		{
			SetVersion(MGetServerConfig()->Version.Revision, 16);
			return;
		}

		std::pair<const char*, u32*> Array[] = {
			{"major", &MGetServerConfig()->Version.Major},
			{"minor", &MGetServerConfig()->Version.Minor},
			{"patch", &MGetServerConfig()->Version.Patch},
		};
		std::pair<const char*, u32*>* Element = nullptr;
		for (auto&& pair : Array)
		{
			if (!_stricmp(VersionTypeString, pair.first))
			{
				Element = &pair;
				break;
			}
		}
		if (!Element)
		{
			MLog("Invalid version string %s.\n", VersionTypeString);
			return;
		}

		SetVersion(*Element->second, 10);
	});

	AddConsoleCommand("setversionchecking", 0, 1,
		"Toggles client version checking on login.",
		"setversionchecking [0/1]",
		"",
		[] {
		if (NumArguments == 0) {
			MGetServerConfig()->VersionChecking = !MGetServerConfig()->VersionChecking;
		}
		else {
			MGetServerConfig()->VersionChecking = atoi(Splits[1].c_str()) != 0;
		}
		MLog("Set version checking to %s.\n", MGetServerConfig()->VersionChecking ? "true" : "false");
	});

	AddConsoleCommand("getversion", 0, 1,
		"Outputs the current version.",
		"getversion",
		"",
		[] {
		MLog("%d.%d.%d-%X\n",
			MGetServerConfig()->Version.Major, MGetServerConfig()->Version.Minor,
			MGetServerConfig()->Version.Patch, MGetServerConfig()->Version.Revision);
	});
}

void MBMatchServer::OnInput(const std::string & Input)
{
	Line = Input;

	Splits.clear();

	size_t LastPos = 0;
	for (size_t Pos = 0; Pos < Input.length(); ++Pos)
	{
		if (Input[Pos] == ' ') {
			Splits.emplace_back(Input.begin() + LastPos, Input.begin() + Pos);
			while (Input[Pos] == ' ' && Pos < Input.length())
				++Pos;
			LastPos = Pos;
		}
	}

	if (LastPos < Input.length())
		Splits.emplace_back(Input.begin() + LastPos, Input.end());

	if (Splits.empty())
		return;

	auto it = ConsoleCommandMap.find(Splits[0]);
	if (it == ConsoleCommandMap.end())
	{
		MLog("Unknown command \"%s\"\n", Splits[0].c_str());
		return;
	}

	auto&& Command = it->second;

	NumArguments = int(Splits.size()) - 1;
	if ((Command.MinArgs != NoArgumentLimit && NumArguments < Command.MinArgs) ||
		(Command.MaxArgs != NoArgumentLimit && NumArguments > Command.MaxArgs))
	{
		MLog("Incorrect number of arguments to %s, valid range is %d-%d, got %d\n",
			it->first.c_str(), Command.MinArgs, Command.MaxArgs, NumArguments);
		return;
	}

	Command.Callback();
}
