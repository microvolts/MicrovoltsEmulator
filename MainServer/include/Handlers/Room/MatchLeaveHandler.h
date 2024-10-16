#ifndef MATCH_LEAVE_HANDLER_H
#define MATCH_LEAVE_HANDLER_H

#include "../../Network/MainSession.h"
#include "../../Classes/RoomsManager.h"
#include "Network/Packet.h"
#include "../../Network/MainSessionManager.h"

namespace Main
{
	namespace Handlers
	{	
		inline void handleMatchLeave(const Common::Network::Packet& request, Main::Network::Session& session, Main::Network::SessionsManager& sessionsManager, Main::Classes::RoomsManager& roomsManager)
		{
			if (Main::Classes::Room* room = roomsManager.getRoomByNumber(session.getRoomNumber()))
			{
				if (room->isHost(session.getAccountInfo().uniqueId))
				{
					if (room->removeHostFromMatch())
					{
						roomsManager.removeRoom(room->getRoomNumber());
					}
					else
					{
						// handle error?
					}
				}
				else
				{
					// Send leave match packet to client 
					auto uniqueId = session.getAccountInfo().uniqueId;
					Common::Network::Packet response;
					response.setTcpHeader(request.getSession(), Common::Enums::USER_LARGE_ENCRYPTION);
					response.setCommand(request.getOrder(), 0, 0, 0);
					response.setData(reinterpret_cast<std::uint8_t*>(&uniqueId), sizeof(uniqueId));
					roomsManager.broadcastToRoom(room->getRoomNumber(), response);

					// Update server status for this player
					session.setIsInMatch(false);
					room->setStateFor(session.getAccountInfo().uniqueId, Common::Enums::STATE_WAITING);
				}
			}
		}
	}
}

#endif
