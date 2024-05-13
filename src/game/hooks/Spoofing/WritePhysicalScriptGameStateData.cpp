#include "core/commands/BoolCommand.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/backend/FiberPool.hpp"
#include "game/features/Features.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/rdr/Natives.hpp"


namespace YimMenu::Features
{
	BoolCommand _HideVehicleGod("hidevehiclegod", "Hide Vehicle Godmode", "Hides Vehicle Godmode from other players");
}

namespace YimMenu::Hooks
{
	void Spoofing::WritePhysicalScriptGameStateData(__int64 iface, __int64* node)
	{
		for (int i = 0; i < 15; i++)
		{
			*(node + i) = true;
			FiberPool::Push([=] {
				int proofs = ENTITY::_GET_ENTITY_PROOFS(Self::Veh);
				LOG(VERBOSE) << proofs;
			});
		}
	}
}