#include "core/hooking/DetourHook.hpp"
#include "game/hooks/Hooks.hpp"
#include "util/Storage/Spoofing.hpp"

#include <entity/fwEntity.hpp>
#include <network/netObject.hpp>
#include <network/sync/vehicle/CVehicleProximityMigrationData.hpp>


namespace YimMenu::Hooks
{
	void Spoofing::WriteVPMData(rage::netObject* vehicle, CVehicleProximityMigrationData* data)
	{
		BaseHook::Get<Spoofing::WriteVPMData, DetourHook<decltype(&Spoofing::WriteVPMData)>>()->Original()(vehicle, data);

		LOG(VERBOSE) << "HOOK HIT";

		if (auto it = g_SpoofingStorage.remote_teleports.find(vehicle->m_ObjectId);
		    it != g_SpoofingStorage.remote_teleports.end())
		{
			LOG(VERBOSE) << "FOUND TP";
			auto ptr                      = Entity(it->second.GetPed().GetHandle()).GetPointer<rage::fwEntity*>();
			data->m_PassengersActive[0]   = true;
			data->m_PassengerObjectIds[0] = ptr->m_NetObject->m_ObjectId;
			data->m_Position              = it->second.GetPed().GetPosition();
			data->m_Velocity.x            = 1;
			data->m_Velocity.y            = 1;
			data->m_Velocity.z            = 1;
		}
	}
}