#ifndef ROOM_START_HANDLER_HEADER
#define ROOM_START_HANDLER_HEADER

#include "../../Network/MainSession.h"
#include "../../../include/Structures/AccountInfo/MainAccountInfo.h"
#include "Network/Packet.h"
#include "../../Classes/RoomsManager.h"
#include <Utils/IPC_Structs.h>

namespace Main
{
	namespace Handlers
	{
		inline void handleRoomStart(const Common::Network::Packet& request, Main::Network::Session& session, Main::Classes::RoomsManager& roomsManager,
			std::uint64_t timeSinceLastServerRestart)
		{
			if (Main::Classes::Room* room = roomsManager.getRoomByNumber(session.getRoomNumber()))
			{
				Common::Network::Packet response;
				response.setTcpHeader(request.getSession(), Common::Enums::USER_LARGE_ENCRYPTION);
				auto selfUniqueId = session.getAccountInfo().uniqueId;

				// Do NOT touch this part, or you risk breaking the match-start mechanism!
				if (request.getExtra() == 38) // host or non-host clicks on "start" button (n.b: SingleWave's extra is 6)
				{
					response.setCommand(request.getOrder(), 0, 38, room->getRoomSettings().map);
					response.setData(reinterpret_cast<std::uint8_t*>(&selfUniqueId), sizeof(selfUniqueId));
					room->broadcastToRoom(response);

					if (room->isHost(selfUniqueId))
					{
						Utils::MapInfo mapInfo{ room->getRoomSettings().map, selfUniqueId.session };
						Utils::IPCManager::ipc_mainToCast(mapInfo, std::to_string(room->getRoomNumber()), "map_info");
					}
					else if (room->hasMatchStarted())
					{
						room->setStateFor(selfUniqueId, static_cast<Common::Enums::PlayerState>(11));
						session.setIsInMatch(true);
					}
				}
				else if (request.getExtra() == 41)
				{
					session.setIsInMatch(true);

					if (room->isHost(selfUniqueId)) // broadcast the tick to the room
					{
						std::uint64_t roomTick = Details::getUtcTimeMs() - timeSinceLastServerRestart;
						response.setCommand(258, 0, 1, 0);
						response.setData(reinterpret_cast<std::uint8_t*>(&roomTick), sizeof(roomTick));
						room->broadcastToRoom(response);
						room->startMatch(selfUniqueId); // don't move this anywhere else!
					}
					else // Tell the other players in the match that we joined
					{
						response.setCommand(415, 0, 1, 0);
						response.setData(reinterpret_cast<std::uint8_t*>(&selfUniqueId), sizeof(selfUniqueId));
						room->broadcastToRoom(response);
					}
				}
			}
		}
	}
}

#endif
