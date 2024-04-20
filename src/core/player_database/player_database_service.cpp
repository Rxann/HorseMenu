#include "player_database_service.hpp"

namespace YimMenu
{
	player_database_service::player_database_service() :
	    m_file(FileMgr::GetProjectFile("./database.json"))
	{
		load();
	}

	player_database_service::~player_database_service()
	{
	}

	void player_database_service::save()
	{
		nlohmann::json data;

		for (auto& [rid, player] : m_players)
		{
			data[rid] = *player;
		}

		std::ofstream file_stream(m_file.Path());
		file_stream << data;
		file_stream.close();
	}

	void player_database_service::load()
	{
		FileMgr::CreateFileIfNotExists(m_file);

		std::filesystem::path path_ref = m_file;

		std::ifstream file_stream(path_ref);

		nlohmann::json data;
		file_stream >> data;
		file_stream.close();

		for (auto& [key, value] : data.items())
		{
			auto plyr                  = value.get<persistent_player>();
			auto shared                = std::make_shared<persistent_player>(plyr);
			m_players[std::stoll(key)] = shared;

			std::string lower = plyr.name;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
			m_sorted_players[lower] = shared;
		}
	}


	std::unordered_map<uint64_t, std::shared_ptr<persistent_player>>& player_database_service::get_players()
	{
		return m_players;
	}

	std::map<std::string, std::shared_ptr<persistent_player>>& player_database_service::get_sorted_players()
	{
		return m_sorted_players;
	}

	std::shared_ptr<persistent_player> player_database_service::add_player(std::uint64_t rid, std::string name)
	{
		std::string lower = name;
		std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
		if (m_players.contains(rid))
		{
			m_sorted_players.erase(lower);
		}

		auto player    = std::make_shared<persistent_player>(name.data(), rid);
		m_players[rid] = player;

		m_sorted_players[lower] = player;

		return player;
	}

	std::shared_ptr<persistent_player> player_database_service::get_player_by_rockstar_id(uint64_t rockstar_id)
	{
		if (m_players.contains(rockstar_id))
			return m_players[rockstar_id];
		return nullptr;
	}

	uint64_t player_database_service::get_rid_from_player(Player player)
	{
		// Temporary, change in favor of Player::GetRid() when that is merged
		return player.GetHandle()->m_PlayerInfo->m_GamerInfo.m_GamerHandle.m_rockstar_id;
	}

	// Probably Unneeded, YimMenu::Player can take CNetGamePlayer as a constructor
	uint64_t player_database_service::get_rid_from_cnetgameplayer(CNetGamePlayer* player)
	{
		return player->m_PlayerInfo->m_GamerInfo.m_GamerHandle.m_rockstar_id;
	}

	std::shared_ptr<persistent_player> player_database_service::get_or_create_player(Player player)
	{
		uint64_t rid = get_rid_from_player(player);
		if (m_players.contains(rid))
			return m_players[rid];
		else
		{
			std::shared_ptr<persistent_player> player_ptr = add_player(rid, player.GetName());
			save();
			return player_ptr;
		}
	}

	void player_database_service::update_rockstar_id(uint64_t old, uint64_t _new)
	{
		auto player  = m_players.extract(old);
		player.key() = _new;

		m_players.insert(std::move(player));
	}

	void player_database_service::set_selected(std::shared_ptr<persistent_player> selected)
	{
		m_selected = selected;
	}

	std::shared_ptr<persistent_player> player_database_service::get_selected()
	{
		return m_selected;
	}

	void player_database_service::add_infraction_to_player(std::shared_ptr<persistent_player> player, eInfractions infraction)
	{
		// Logic so the infractions don't get too filled up
		if (player->infractions.count(int(infraction)) < 10)
			player->infractions.insert(int(infraction));
	}


	void player_database_service::remove_all_infractions_from_player(std::shared_ptr<persistent_player> player)
	{
		for (auto& infraction : player->infractions)
		{
			player->infractions.erase(infraction);
		}
	}

	bool player_database_service::is_player_in_database(Player player)
	{
		uint64_t rid = get_rid_from_player(player);
		if (m_players.contains(rid))
			return true;
		else
			return false;
	}
}