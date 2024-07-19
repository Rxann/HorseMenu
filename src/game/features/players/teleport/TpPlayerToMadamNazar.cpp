#include "game/commands/PlayerCommand.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/Self.hpp"
#include "game/rdr/Natives.hpp"
#include "game/rdr/ScriptGlobal.hpp"
#include "util/Teleport.hpp"

namespace YimMenu::Features
{
	class TpPlayerToMadamNazar : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			// search for "MPSW_LOCATION_00" in long_update
			auto nazarLocation = ScriptGlobal(1051832).At(4681).As<Vector3*>();
			auto coords        = *nazarLocation;

			Teleport::TeleportPlayerToCoords(player, coords);
		}
	};

	static TpPlayerToMadamNazar _TpPlayerToMadamNazar{"tpplayertomadamnazar", "Teleport To Madam Nazar", "Teleports the player to Madam Nazar's current location"};
}