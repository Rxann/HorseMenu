#include "core/commands/BoolCommand.hpp"
#include "game/features/Features.hpp"
#include "game/pointers/Pointers.hpp"


namespace YimMenu::Features
{
	class ForceRelayConnection : public BoolCommand
	{
		using BoolCommand::BoolCommand;

		virtual void OnEnable() override
		{
			*Pointers.ForceRelayConnection = true;
		}

		virtual void OnDisable() override
		{
			*Pointers.ForceRelayConnection = false;
		}
	};

	static ForceRelayConnection _ForceRelayConnection{"forcerelayconnection", "Force Relay Connection", "Forces you to connect through Rockstar relay servers"};
}
