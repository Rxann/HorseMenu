#include "core/commands/Command.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/Self.hpp"
#include "game/rdr/Natives.hpp"
#include "game/rdr/ScriptGlobal.hpp"
#include "util/Teleport.hpp"

namespace YimMenu::Features
{
	class TpToMadamNazar : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			// search for "MPSW_LOCATION_00" in long_update
			auto nazarLocation = ScriptGlobal(1051832).At(4681).As<Vector3*>();
			auto coords        = *nazarLocation;

			Teleport::TeleportEntity(Self::GetPed().GetHandle(), coords, true);
		}
	};

	static TpToMadamNazar _TpToMadamNazar{"tptomadamnazar", "Teleport To Madam Nazar", "Teleports to Madam Nazar's current location"};
}