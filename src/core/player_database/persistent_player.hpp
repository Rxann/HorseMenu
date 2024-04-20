#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_set>


namespace YimMenu
{
	struct persistent_player
	{
		std::string name;
		bool is_modder  = false;
		bool is_trusted = false;
		std::unordered_set<int> infractions;
		uint64_t rid;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(persistent_player, name, is_modder, is_trusted, infractions, note, rid);

		static const char* get_infraction_description(int infraction)
		{
			return GetInstance().get_infraction_description_impl(infraction);
		}

	private:
		const char* get_infraction_description_impl(int infraction);
		static persistent_player& GetInstance()
		{
			static persistent_player Instance;
			return Instance;
		}
	};

	enum class eInfractions : int
	{
		CUSTOM_REASON              = 0,
		CRASH                      = 1,
		SPOOFED_NAME               = 2,
		UNKNOWN_PED_CRASH          = 3,
		INVALID_OBJECT_CRASH       = 4,
		MISMATCHED_MODEL_CRASH     = 5,
		MISMATCHED_PLAYER_MODEL    = 6,
		PLAYER_MODEL_SWITCH        = 7,
		MISMATCHED_VEHICLE_MODEL   = 8,
		VEHICLE_FLOOD              = 9,
		ATTACHMENT                 = 10,
		REMOTE_TELEPORT            = 11,
		UNKNOWN_TASK               = 12,
		PED_ATTACHMENT             = 13,
		INVALID_PROPSET            = 14,
		MISMATCHED_PED_MODEL_CRASH = 15
	};

}