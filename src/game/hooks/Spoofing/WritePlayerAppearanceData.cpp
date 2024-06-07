#include "core/commands/BoolCommand.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/rdr/Natives.hpp"
#include "util/Storage/Spoofing.hpp"

#include <network/sync/player/CPlayerAppearanceData.hpp>


namespace YimMenu::Features
{
	BoolCommand _SpoofModel("spoofmodel", "Spoof Model", "Spoofs your player model for other players!");
}

namespace YimMenu::Hooks
{
	void Spoofing::WritePlayerAppearanceData(rage::netObject* player, CPlayerAppearanceData* data)
	{
		BaseHook::Get<Spoofing::WritePlayerAppearanceData, DetourHook<decltype(&Spoofing::WritePlayerAppearanceData)>>()->Original()(player, data);

		if (Features::_SpoofModel.GetState())
		{
			if (STREAMING::IS_MODEL_VALID((Hash)g_SpoofingStorage.spoofed_model))
			{
				data->m_ModelHash = g_SpoofingStorage.spoofed_model;
			}
		}
	}
}