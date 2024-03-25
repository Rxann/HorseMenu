#pragma once
#include "persistent_player.hpp"

namespace YimMenu
{
	static std::unordered_map<eInfractions, const char*> infraction_desc = {{eInfractions::SPOOFED_NAME, "Spoofed Name"}, {eInfractions::ATTACHMENT, "Invalid Attachment"}, {eInfractions::INVALID_OBJECT_CRASH, "Invalid Object Crash"}, {eInfractions::INVALID_PROPSET, "Invalid Propset"}, {eInfractions::MISMATCHED_MODEL_CRASH, "Mismatched Object Model Crash"}, {eInfractions::MISMATCHED_PLAYER_MODEL, "Mismatched Player Model Crash"}, {eInfractions::MISMATCHED_VEHICLE_MODEL, "Mismatched Vehicle Model Crash"}, {eInfractions::PED_ATTACHMENT, "Ped Attachment"}, {eInfractions::PLAYER_MODEL_SWITCH, "Player Model Switch Crash"}, {eInfractions::REMOTE_TELEPORT, "Remote Teleport"}, {eInfractions::UNKNOWN_PED_CRASH, "Unknown Ped Crash"}, {eInfractions::UNKNOWN_TASK, "Unknown Task"}, {eInfractions::VEHICLE_FLOOD, "Vehicle Flood"}, {eInfractions::MISMATCHED_PED_MODEL_CRASH, "Mismatched Ped Model Crash"}};

	const char* persistent_player::get_infraction_description_impl(int infraction)
	{
		if (eInfractions(infraction) == eInfractions::CUSTOM_REASON)
		{
			return "Custom Infraction";
		}
		return infraction_desc.at(eInfractions(infraction));
	}
}