#ifndef _MYENGINE_NETWORKCOMPONENTS_H_
#define _MYENGINE_NETWORKCOMPONENTS_H_

#include "engine/Defines.h"
#include "engine/Component.h"
#include "shared/NetworkDefs.h"

#include <vector>

typedef unsigned long long SOCKET;

namespace MYENGINE
{

	struct NetworkComponent : public Component
	{
	public:
		NetworkComponent(weak(GameObject) _parentObject);
		~NetworkComponent();

		void Update() override;

		/**
		* Try to connect to an available server.
		*
		* @returns whether a connection is successfully made.
		**/
		bool TryConnect();

		/**
		* Add a new message to the outgoing queue.
		*
		* @param recipient : The ID of the desired recipient.
		* @param message : The body of the message.
		**/
		void Send(byte recipient, std::string message);

		/**
		* @returns whether this NetworkComponent has been disconnected.
		**/
		bool Disconnected() { return m_disconnected; }

	protected:
		void HasDisconnected();

		SOCKET m_socket;
		byte m_clientID = 0;
		char m_incoming[MSG_TOTAL_LENGTH] = { 0 };
		char m_outgoing[MSG_TOTAL_LENGTH] = { 0 };

		std::vector<Message> m_incomingQueue;
		std::vector<Message> m_outgoingQueue;

		bool m_disconnected;
	};

} // namespace MYENGINE

#endif
