#include "core/frontend/Notifications.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/backend/Protections.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/rdr/Natives.hpp"
#include "network/CNetGamePlayer.hpp"


namespace YimMenu::Hooks
{
	void Info::PlayerHasJoined(CNetGamePlayer* player)
	{
		BaseHook::Get<Info::PlayerHasJoined, DetourHook<decltype(&Info::PlayerHasJoined)>>()->Original()(player);

		YimMenu::Protections::IsSpoofedName(g_player_database_service->get_or_create_player(Player(player)));

		LOG(INFO) << player->GetName() << " is joining your session.";
	}

	void Info::PlayerHasLeft(CNetGamePlayer* player)
	{
		BaseHook::Get<Info::PlayerHasLeft, DetourHook<decltype(&Info::PlayerHasLeft)>>()->Original()(player);

		LOG(INFO) << player->GetName() << " has left your session.";
	}
}