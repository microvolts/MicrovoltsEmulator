#include "../../include/Classes/Room.h"
#include "../../include/Classes/RoomsManager.h"

#include <Utils/Logger.h>

namespace Cast
{
	namespace Classes
	{
		void RoomsManager::addRoom(std::shared_ptr<Cast::Classes::Room> room, std::uint64_t playerId)
		{
			m_playerSessionIdToRoom[playerId] = room;
		}

		void RoomsManager::switchRoomJoinOrExit(Cast::Network::Session& session, std::uint64_t hostSessionId)
		{
			// If the player is inside the room, they must leave.
			// Otherwise, they join it.
			if (m_playerSessionIdToRoom.contains(session.getId())) 
			{
				// The player identified by "session" is inside the room where the host's sessionId equals "roomId", thus the room exists
				removePlayerFromRoom(session.getId());
			}
			else
			{
				// The player just joined a room
				if (m_playerSessionIdToRoom.contains(hostSessionId))
				{
					m_playerSessionIdToRoom[hostSessionId]->addPlayer(&session);
					m_playerSessionIdToRoom[session.getId()] = m_playerSessionIdToRoom[hostSessionId];
				}
			}
		}

		void RoomsManager::broadcastToRoomExceptSelf(std::uint64_t sessionId, Common::Network::Packet& packet)
		{
			if (m_playerSessionIdToRoom.contains(sessionId))
			{
				m_playerSessionIdToRoom[sessionId]->broadcastToRoomExceptSelf(sessionId, packet);
			}
		}

		void RoomsManager::broadcastToRoom(std::uint64_t sessionId, Common::Network::Packet& packet)
		{
			if (m_playerSessionIdToRoom.contains(sessionId))
			{
				m_playerSessionIdToRoom[sessionId]->broadcastToRoom(packet);
			}
		}

		void RoomsManager::playerForwardToHost(std::uint64_t hostSessionId, std::uint64_t senderSessionId, Common::Network::Packet& packet)
		{
			if (m_playerSessionIdToRoom.contains(senderSessionId))
			{
				m_playerSessionIdToRoom[senderSessionId]->playerForwardToHost(hostSessionId, senderSessionId, packet);
			}
		}

		void RoomsManager::hostForwardToPlayer(std::uint64_t hostSessionId, std::uint64_t receiverSessionId, Common::Network::Packet& packet, bool useHostSessionIdInTcpHeader)
		{
			if (m_playerSessionIdToRoom.contains(hostSessionId))
			{
				m_playerSessionIdToRoom[hostSessionId]->hostForwardToPlayer(hostSessionId, receiverSessionId, packet);
			}
		}


		void RoomsManager::setMapFor(std::uint64_t playerId, std::uint32_t map)
		{
			if (!m_playerSessionIdToRoom.contains(playerId)) return;
			auto& room = m_playerSessionIdToRoom[playerId];
			room->setMap(map);
		}

		bool RoomsManager::exists(std::uint64_t playerId)
		{
			return m_playerSessionIdToRoom.contains(playerId);
		}
		
		std::uint32_t RoomsManager::getMapOf(std::uint64_t playerId)
		{
			auto& room = m_playerSessionIdToRoom[playerId];
			return room->getMap();
		}

		void RoomsManager::removePlayerFromRoom(std::uint64_t sessionIdToRemoveFromRoom)
		{
			Utils::Logger& logger = Utils::Logger::getInstance();

			if (m_playerSessionIdToRoom.contains(sessionIdToRemoveFromRoom))
			{
				m_playerSessionIdToRoom[sessionIdToRemoveFromRoom]->removePlayer(sessionIdToRemoveFromRoom);
				m_playerSessionIdToRoom.erase(sessionIdToRemoveFromRoom);
			}
		}

		void RoomsManager::endMatch(std::uint64_t sessionId)
		{
			if (m_playerSessionIdToRoom.contains(sessionId))
			{
				m_playerSessionIdToRoom[sessionId]->endMatch();
			}
		}

		void RoomsManager::leaveAllPlayers(std::uint64_t sessionId)
		{
			if (m_playerSessionIdToRoom.contains(sessionId))
			{
				m_playerSessionIdToRoom[sessionId]->leaveAllPLayers();
			}
		}
	};
}
