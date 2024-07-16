#pragma once
#include "Storage/Spoofing.hpp"
#include "VehicleSpawner.hpp"
#include "common.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/rdr/Entity.hpp"
#include "game/rdr/Natives.hpp"
#include "game/rdr/Player.hpp"
#include "network.hpp"

#include <entity/fwEntity.hpp>
#include <network/CNetObjectMgr.hpp>
#include <network/netObject.hpp>


// TODO: remove this file

namespace YimMenu::Teleport
{
	inline bool LoadGroundAtCoords(rage::fvector3& location)
	{
		constexpr float max_ground_check = 1000.f;
		constexpr int max_attempts       = 300;
		float ground_z                   = location.z;
		int current_attempts             = 0;
		bool found_ground;
		float height;

		do
		{
			found_ground = MISC::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, max_ground_check, &ground_z, FALSE);
			STREAMING::REQUEST_COLLISION_AT_COORD(location.x, location.y, location.z);

			if (current_attempts % 10 == 0)
			{
				location.z += 25.f;
			}

			++current_attempts;

			ScriptMgr::Yield();
		} while (!found_ground && current_attempts < max_attempts);

		if (!found_ground)
		{
			return false;
		}

		if (WATER::GET_WATER_HEIGHT(location.x, location.y, location.z, &height))
		{
			location.z = height;
		}
		else
		{
			location.z = ground_z + 1.f;
		}

		return true;
	}

	// Entity typdef is being ambiguous with Entity class
	inline bool TeleportEntity(int ent, rage::fvector3 coords, bool loadGround = false)
	{
		if (ENTITY::IS_ENTITY_A_PED(ent))
		{
			if (PED::IS_PED_ON_MOUNT(ent))
				ent = PED::GET_MOUNT(ent);
			if (PED::IS_PED_IN_ANY_VEHICLE(ent, false))
				ent = PED::GET_VEHICLE_PED_IS_USING(ent);
		}

		// TODO: request control of entity
		if (loadGround)
		{
			if (LoadGroundAtCoords(coords))
			{
				Entity(ent).SetPosition(coords);
				Notifications::Show("Teleport", "Teleported entity to coords", NotificationType::Success);
			}
		}
		else
		{
			Entity(ent).SetPosition(coords);
			Notifications::Show("Teleport", "Teleported entity to coords", NotificationType::Success);
		}

		return true;
	}

	inline Vector3 GetWaypointCoords()
	{
		if (MAP::IS_WAYPOINT_ACTIVE())
			return MAP::_GET_WAYPOINT_COORDS();

		return Vector3{0, 0, 0};
	}

	inline bool WarpIntoVehicle(int ped, int veh)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(veh) || !ENTITY::DOES_ENTITY_EXIST(ped))
			return false;

		int seat   = -2;
		auto seats = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(veh));

		for (int i = -1; i < seats; i++)
		{
			if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i))
			{
				seat = i;
				break;
			}
		}

		if (seat < -1)
		{
			Notifications::Show("Teleport", "No free seats in vehicle", NotificationType::Error);
			return false;
		}
		else
		{
			PED::SET_PED_INTO_VEHICLE(ped, veh, seat);
			return true;
		}
	}

	inline bool BringPlayer(Player player, Vector3 coords)
	{
		int handle = player.GetPed().GetHandle();

		if (ENTITY::IS_ENTITY_DEAD(handle))
		{
			Notifications::Show("Teleport", "The player you want to teleport is dead!", NotificationType::Error);
			return false;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(handle, true))
		{
			LOG(VERBOSE) << "PED IS IN VEHICLE";

			auto veh = PED::GET_VEHICLE_PED_IS_USING(handle);

			LOG(VERBOSE) << "VEH: " << std::to_string(veh);

			LOG(VERBOSE) << "REQUESTING CONTROL";
			if (Network::RequestControlOfEntity(veh))
			{
				LOG(VERBOSE) << "GOT CONTROL";
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(veh, coords.x, coords.y, coords.z, true, true, true);
			}
			else
			{
				LOG(VERBOSE) << "FAILED TO TAKE CONTROL OF VEH";
				Notifications::Show("Teleport", "Failed to Take Control of Target's Vehicle!", NotificationType::Error);
			}

			return true;
		}
		else
		{
			LOG(VERBOSE) << "SPAWNING VEHICLE";
			auto hnd = SpawnVehicle("coach4", player.GetPed().GetPosition(), 0.0f, false);

			if (!hnd)
			{
				LOG(VERBOSE) << "VEHICLE IS NULL";
				return false;
			}

			auto ent = Entity(hnd);
			auto ptr = ent.GetPointer<rage::fwEntity*>();

			if (!ptr->m_NetObject)
			{
				LOG(VERBOSE) << "NET OBJECT IS NULL";
				return false;
			}

			LOG(VERBOSE) << "SETTING STUFF";
			ENTITY::SET_ENTITY_VISIBLE(hnd, false);
			ENTITY::SET_ENTITY_COLLISION(hnd, false, false);
			ENTITY::FREEZE_ENTITY_POSITION(hnd, true);

			LOG(VERBOSE) << "Emplacing";
			auto vehId = ptr->m_NetObject->m_ObjectId;
			g_SpoofingStorage.remote_teleports.emplace(vehId, player);

			if (player.IsValid() && PED::IS_PED_IN_ANY_VEHICLE(player.GetPed().GetHandle(), false))
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(player.GetPed().GetHandle(), true, true);

			LOG(VERBOSE) << "STARTING LOOP";
			for (int i = 0; i < 30; i++)
			{
				ScriptMgr::Yield(25ms);

				(*Pointers.NetworkObjectMgr)->ChangeOwner(ptr->m_NetObject, player.GetHandle(), 3, true);

				auto newCoords = ENTITY::GET_ENTITY_COORDS(hnd, true, true);
				if (BUILTIN::VDIST(coords.x, coords.y, coords.z, newCoords.x, newCoords.y, newCoords.z) < 20 * 20 && VEHICLE::GET_PED_IN_VEHICLE_SEAT(hnd, 0) == handle)
				{
					LOG(VERBOSE) << "BREAKING";
					break;
				}
			}

			LOG(VERBOSE) << "DELETING";
			ent.Delete();

			LOG(VERBOSE) << "ERASING";
			std::erase_if(g_SpoofingStorage.remote_teleports, [vehId](auto& obj) {
				return obj.first == vehId;
			});

			return true;
		}
	}
}
