#pragma once
#include "core/frontend/Notifications.hpp"
#include "core/player_database/player_database_service.hpp"
#include "game/rdr/Player.hpp"


namespace YimMenu
{
	class Protections
	{
		Player m_SyncingPlayer{nullptr};

		static Protections& GetInstance()
		{
			static Protections Instance;
			return Instance;
		}

	public:
		static Player& GetSyncingPlayer()
		{
			return GetInstance().m_SyncingPlayer;
		}

		static void SetSyncingPlayer(Player player)
		{
			GetInstance().m_SyncingPlayer = player;
		}

		bool IsSpoofedNameImpl(std::shared_ptr<persistent_player> player);

		static bool IsSpoofedName(std::shared_ptr<persistent_player> player)
		{
			return GetInstance().IsSpoofedNameImpl(player);
		}
	};
}