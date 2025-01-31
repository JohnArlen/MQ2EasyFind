
#pragma once

#include <mq/Plugin.h>

#include <spdlog/common.h>

#include <yaml-cpp/yaml.h>

#include <memory>

enum class ConfiguredColor
{
	AddedLocation = 0,
	ModifiedLocation,

	MaxColors,
};
const char* GetConfiguredColorName(ConfiguredColor color);
const char* GetConfiguredColorDescription(ConfiguredColor color);

enum class ConfiguredGroupPlugin
{
	None = 0,
	Auto,
	EQBC,
	Dannet,

	Max,
};
const char* GetGroupPluginPreferenceString(ConfiguredGroupPlugin plugin);

namespace spdlog {
	namespace sinks {
		class sink;
	}
}

struct GuildHallClickyItem
{
	std::string zoneName;
	std::string zoneShortName;
	std::string guildClickyCommand;
	std::string itemName;
	std::string menuText;
	bool enabled = false;
};
using GuildHallClickies = std::vector<GuildHallClickyItem>;

class EasyFindConfiguration
{
public:
	EasyFindConfiguration(const std::string& configDirectory);
	~EasyFindConfiguration();

	void LoadSettings();
	void SaveSettings();

	void ReloadSettings();
	void ResetSettings();

	void SetColor(ConfiguredColor color, MQColor value);
	MQColor GetColor(ConfiguredColor color) const;
	MQColor GetDefaultColor(ConfiguredColor color) const;

	void SetLogLevel(spdlog::level::level_enum level);
	spdlog::level::level_enum GetLogLevel() const;

	void SetNavLogLevel(spdlog::level::level_enum level);
	spdlog::level::level_enum GetNavLogLevel() const;

	void SetColoredFindWindowEnabled(bool colorize);
	bool IsColoredFindWindowEnabled() const { return m_coloredFindWindowEnabled; }

	void SetDistanceColumnEnabled(bool enable);
	bool IsDistanceColumnEnabled() const { return m_distanceColumnEnabled; }

	void SetSilentGroupCommands(bool silent);
	bool IsSilentGroupCommands() const { return m_silentGroupCommands; }

	void SetVerboseMessages(bool verbose);
	bool IsVerboseMessages() const { return m_verboseMessages; }

	void SetIgnoreZoneConnectionDataEnabled(bool ignore);
	bool IsIgnoreZoneConnectionDataEnabled() const { return m_ignoreZoneConnectionDataEnabled; }

	// transfer types
	void RefreshTransferTypes();
	bool IsSupportedTransferType(int transferTypeIndex) const;
	bool IsDisabledTransferType(int transferTypeIndex) const;
	void SetDisabledTransferType(int transferTypeIndex, bool disabled);

	// guild hall clickies
	void SetEnabledGuildHallClicky(GuildHallClickyItem* clicky, bool enabled);
	GuildHallClickies GetAllGuildHallClickyItems() const;
	void DetermineGuildHallClickies();
	const bool CurrentlyInAGuildHall() const;
	bool RequireReload();
	const bool GetUseGuildClickies() const;
	void SetUseGuildClickies(bool useGuildClickies);
	const bool GetUseGuildClickyLua() const;
	void SetUseGuildClickyLua(bool useGuildClickyLua);

	// group execution behaviors
	bool IsEQBCLoaded() const { return m_eqbcLoaded; }
	bool IsDannetLoaded() const { return m_dannetLoaded; }

	ConfiguredGroupPlugin GetPreferredGroupPlugin() const;        // returns user preference.
	void SetPreferredGroupPlugin(ConfiguredGroupPlugin p);

	ConfiguredGroupPlugin GetActiveGroupPlugin() const;           // returns the actual thing to use.

	void HandlePluginChange(std::string_view pluginName, bool loaded);

	int GetNavDistance() const { return 15; }

private:
	void LoadDisabledTransferTypes();
	void LoadEnabledGuildHallClickies();
	void LoadGuildHallClickiesFile();

private:
	std::string m_easyfindDir;
	std::string m_configFile;
	YAML::Node m_configNode;

	bool m_guildHallClickiesLoaded = false;
	GuildHallClickies m_allGuildHallClickies;
	std::vector<std::string> m_enabledGuildHallClickies;   // user pref
	bool m_requireConnectionReload;
	bool m_useGuildClickyLua;
	bool m_useGuildClickies;

	std::vector<std::string> m_disabledTransferTypesPrefs; // user pref
	std::vector<bool> m_supportedTransferTypes;            // hardcoded disabled, converted to index when data is loaded
	std::vector<bool> m_disabledTransferTypes;             // converted to index when data is loaded

	std::shared_ptr<spdlog::sinks::sink> m_chatSink;
	std::array<MQColor, (size_t)ConfiguredColor::MaxColors> m_configuredColors;

	spdlog::level::level_enum m_navLogLevel = spdlog::level::err;

	bool m_eqbcLoaded = false;
	bool m_dannetLoaded = false;
	ConfiguredGroupPlugin m_groupPluginSelection = ConfiguredGroupPlugin::Auto;

	bool m_distanceColumnEnabled = true;
	bool m_coloredFindWindowEnabled = true;
	bool m_silentGroupCommands = true;
	bool m_verboseMessages = false;
	bool m_ignoreZoneConnectionDataEnabled = false;
};

extern EasyFindConfiguration* g_configuration;
