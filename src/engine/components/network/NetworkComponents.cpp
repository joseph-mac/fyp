#include "NetworkComponents.h"

#include "engine/managers/Manager.h"
#include "engine/GameObject.h"
#include "engine/managers/InputManager.h"
#include "engine/Exception.h"
#include "engine/Component.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#undef GetObject

namespace MYENGINE
{
	NetworkComponent::NetworkComponent(weak(GameObject) _parentObject)
		: Component(_parentObject, "NETWORK_COMPONENT")
	{
		m_disconnected = true;

		// WSA setup
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			throw Exception("WSAStartup failed");
		}

		m_incomingQueue = {};
		m_outgoingQueue = {};

		if (!TryConnect())
		{
			std::cout << "NetworkComponent cannot find a server at this time :(" << std::endl;
		}
	}

	NetworkComponent::~NetworkComponent()
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
		}

		// WSA cleanup
		WSACleanup();
	}

	bool NetworkComponent::TryConnect()
	{
		if (!m_disconnected)
		{
			return true;
		}

		std::cout << "NetworkComponent trying to connect to server..." << std::endl;

		// prep
		struct addrinfo hints = { 0 };
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		PADDRINFOA addressInfo;
		int iResult = getaddrinfo(NULL, "8080", &hints, &addressInfo);
		if (iResult != 0)
		{
			return false;
			//throw Exception("getaddrinfo failed");
		}

		// connect
		m_socket = INVALID_SOCKET;

		for (auto ptr = addressInfo; ptr != NULL; ptr = ptr->ai_next)
		{
			m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (m_socket == INVALID_SOCKET)
			{
				return false;
				//throw Exception("socket creation failed");
			}

			int connectResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (connectResult != SOCKET_ERROR)
			{
				break;
			}

			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		unsigned long mode = 1;
		int res = ioctlsocket(m_socket, FIONBIO, &mode);
		if (res != NO_ERROR)
		{
			return false;
			//throw Exception("Non-blocking setup failed");
		}

		m_disconnected = false;
		return true;
	}

	void NetworkComponent::Update()
	{
		ConceptState connectConcept = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_NETWORK_CONNECT");
		if (m_disconnected && connectConcept.changed && connectConcept.value != 0)
		{
			if (!TryConnect())
			{
				std::cout << "NetworkComponent cannot find a server at this time :(" << std::endl;
			}
		}

		if (!m_disconnected)
		{
			ConceptState testSendConcept = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_NETWORK_SEND_TEST");
			if (testSendConcept.changed && testSendConcept.value != 0)
			{
				Send(0, "My message is Y!");
			}


			// clear m_incoming
			for (int i = 0; i < MSG_TOTAL_LENGTH; i++)
			{
				m_incoming[i] = MSG_NULL_CHAR;
			}


			// check if still connected
			int recvResult = recv(m_socket, m_incoming, MSG_TOTAL_LENGTH, MSG_PEEK);
			if (recvResult == 0)
			{
				// connection closed
				HasDisconnected();
				return;
			}
			else if (recvResult == SOCKET_ERROR)
			{
				int wsaError = WSAGetLastError();
				if (wsaError != WSAEWOULDBLOCK)
				{
					// connection error
					HasDisconnected();
					return;
				}
			}


			// recv
			recvResult = recv(m_socket, m_incoming, MSG_TOTAL_LENGTH, 0);
			if (recvResult == 0)
			{
				// connection closed
				HasDisconnected();
				return;
			}
			else if (recvResult == SOCKET_ERROR)
			{
				int wsaError = WSAGetLastError();
				if (wsaError != WSAEWOULDBLOCK)
				{
					// connection error
					HasDisconnected();
					return;
				}
			}
			else
			{
				// recv success
				std::string incomingString(MSG_TOTAL_LENGTH, MSG_NULL_CHAR);
				incomingString.insert(0, m_incoming, MSG_TOTAL_LENGTH);

				m_incomingQueue.push_back(Message(incomingString));
			}

			while (!m_incomingQueue.empty())
			{
				if (m_clientID == 0)
				{
					m_clientID = m_incomingQueue.back().m_recipientID;
				}

				m_incomingQueue.back().Print();
				m_incomingQueue.pop_back();
			}

			while (!m_outgoingQueue.empty())
			{
				// send
				int sendResult = send(m_socket, m_outgoingQueue.front().m_raw.c_str(), MSG_TOTAL_LENGTH, 0);
				if (sendResult == 0)
				{
					// connection closed
					HasDisconnected();
					return;
				}
				else if (sendResult == SOCKET_ERROR)
				{
					int wsaError = WSAGetLastError();
					if (wsaError != WSAEWOULDBLOCK)
					{
						// connection error
						HasDisconnected();
						return;
					}
				}
				else
				{
					// send success
					std::cout << "Send success!" << std::endl;
					m_outgoingQueue.erase(m_outgoingQueue.begin());
				}
			}
		}
	}

	void NetworkComponent::HasDisconnected()
	{
		m_disconnected = true;
		std::cout << "NetworkComponent belonging to '" << GetObject()->GetName() << "' has been disconnected. It must be manually told to attempt to reconnect." << std::endl;
	}

	void NetworkComponent::Send(byte recipient, std::string message)
	{
		m_outgoingQueue.push_back(Message(m_clientID, recipient, message));
	}

} // namespace MYENGINE
