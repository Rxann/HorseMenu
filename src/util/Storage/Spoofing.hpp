#pragma once
#include "game/rdr/Player.hpp"

#include <string>
#include <unordered_map>


namespace YimMenu
{
	struct Spoofing
	{
		std::string spoofed_name;
		//std::unordered_map<uint16_t, Player> remote_teleports;

		struct RemotePlayerTeleport
		{
			std::int16_t m_PlayerNetId;
			rage::fvector3 m_Position;
		};

		std::unordered_map<uint16_t, RemotePlayerTeleport> m_RemotePlayerTeleports;
	};
	inline YimMenu::Spoofing g_SpoofingStorage;
}