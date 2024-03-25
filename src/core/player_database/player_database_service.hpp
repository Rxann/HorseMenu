#pragma once
#include "core/filemgr/File.hpp"
#include "core/filemgr/FileMgr.hpp"
#include "game/rdr/Player.hpp"
#include "network/CNetGamePlayer.hpp"
#include "network/rlGamerHandle.hpp"
#include "network/rlGamerInfo.hpp"
#include "persistent_player.hpp"
#include "player/CPlayerInfo.hpp"

#include <exception>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <utility>


namespace YimMenu
{
	class player_database_service
	{
		std::unordered_map<uint64_t, std::shared_ptr<persistent_player>> m_players;
		std::map<std::string, std::shared_ptr<persistent_player>> m_sorted_players;
		std::shared_ptr<persistent_player> m_selected = nullptr;

	public:
		File m_file;
		player_database_service();
		~player_database_service();

		void save();
		void load();

		std::unordered_map<uint64_t, std::shared_ptr<persistent_player>>& get_players();
		std::shared_ptr<persistent_player> get_or_create_player(Player player);
		std::map<std::string, std::shared_ptr<persistent_player>>& get_sorted_players();
		uint64_t get_rid_from_player(Player player);
		uint64_t get_rid_from_cnetgameplayer(CNetGamePlayer* player);
		void set_selected(std::shared_ptr<persistent_player> selected);
		std::shared_ptr<persistent_player> get_selected();
		void update_rockstar_id(uint64_t old, uint64_t _new);
		std::shared_ptr<persistent_player> add_player(std::uint64_t rid, std::string name);
		std::shared_ptr<persistent_player> get_player_by_rockstar_id(uint64_t rockstar_id);
		void add_infraction_to_player(std::shared_ptr<persistent_player> player, eInfractions infraction);
		void remove_all_infractions_from_player(std::shared_ptr<persistent_player> player);
		bool is_player_in_database(YimMenu::Player player);
	};

	inline player_database_service* g_player_database_service;
}