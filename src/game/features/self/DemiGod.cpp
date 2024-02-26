#include "core/commands/LoopedCommand.hpp"
#include "game/features/Features.hpp"
#include "game/rdr/Enums.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::Features
{
	class DemiGod : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (PED::GET_PED_MAX_HEALTH(Self::PlayerPed) < ENTITY::GET_ENTITY_HEALTH(Self::PlayerPed))
			{
				ENTITY::SET_ENTITY_HEALTH(Self::PlayerPed, PED::GET_PED_MAX_HEALTH(Self::PlayerPed), false);
				return;
			}
		}
	};

	static DemiGod _DemiGod{"demigod", "DemiGod", "Blocks all incoming damage, but avoids standard methods detected by other modders."};
}