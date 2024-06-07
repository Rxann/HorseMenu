#pragma once
#include <string>

namespace YimMenu
{
	struct Spoofing
	{
		std::string spoofed_name;
		uint32_t spoofed_model;
	};
	inline YimMenu::Spoofing g_SpoofingStorage;
}