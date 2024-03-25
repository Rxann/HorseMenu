#include "Protections.hpp"

namespace YimMenu
{
	bool Protections::IsSpoofedNameImpl(std::shared_ptr<persistent_player> player)
	{
		if (player->name.data()[0] == '~')
		{
			g_player_database_service->add_infraction_to_player(player, eInfractions::SPOOFED_NAME);
			Notifications::Show("Protections", std::string("Name Spoofing is detected from ").append(player->name), NotificationType::Warning);
			return true;
		}
		return false;
	}
}