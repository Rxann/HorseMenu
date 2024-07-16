#pragma once
#include "game/rdr/Player.hpp"

#include <string>
#include <unordered_map>


namespace YimMenu
{
	struct Spoofing
	{
		std::string spoofed_name;
		std::unordered_map<uint16_t, Player> remote_teleports;
	};
	inline YimMenu::Spoofing g_SpoofingStorage;
}