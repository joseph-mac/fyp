#pragma once

#include "shared/NetworkDefs.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

struct Client;
struct Message;
void ListenToClient(Client client);
bool Send(Message message);

/* global resources */
static bool closeServer = false;
static std::mutex coutMutex;
static std::mutex allClientsMutex;
static std::mutex clientListenerThreadsMutex;

/* Vector of all clients */
static std::vector<Client> allClients = {};

/* Vector of threads which will listen for messages from each client */
static std::vector<std::thread> clientListenerThreads;


static byte nextClientId = 2;
struct Client
{
    Client(SOCKET socket)
    {
        m_id = nextClientId;
        nextClientId++;

        m_socket = socket;

        m_active = true;

        clientListenerThreadsMutex.lock();
        clientListenerThreads.push_back(std::thread(ListenToClient, *this));
        clientListenerThreadsMutex.unlock();
    }
    ~Client() {}

    bool m_active;

    byte m_id;
    SOCKET m_socket;
    sockaddr m_addr;
};


void AcceptClientSockets(SOCKET listeningSocket)
{
    while (!closeServer)
    {
        /* Listen on socket */
        int res = listen(listeningSocket, SOMAXCONN);
        if (res == SOCKET_ERROR)
        {
            closesocket(listeningSocket);
            WSACleanup();
            throw std::runtime_error("listen failed with error");
        }

        /* Accept client socket */
        int clientAddressLen = 16;
        sockaddr clientAddress;
        Client newClient = Client(accept(listeningSocket, &clientAddress, &clientAddressLen));
        newClient.m_addr = clientAddress;
        if (newClient.m_socket == INVALID_SOCKET)
        {
            closesocket(listeningSocket);
            WSACleanup();
            throw std::runtime_error("accept failed with error");
        }

        allClientsMutex.lock();
        allClients.push_back(newClient);
        allClientsMutex.unlock();

        coutMutex.lock();
        std::cout << "/ + / Client connected. Assigned ID is [" << (int)newClient.m_id << "]" /*, address is " << (int)*newClient.m_addr.sa_data*/ << std::endl;
        coutMutex.unlock();

        std::string msg(MSG_TOTAL_LENGTH, MSG_NULL_CHAR);
        msg.insert(0, "You have now connected to the server!");
        Message newClientMsg(0, newClient.m_id, msg);
        if (!Send(newClientMsg))
        {
            std::cout << "/ ! / Send error." << std::endl;
        }
    }
    return;
}

void ListenToClient(Client client)
{
    int recvResult = 1;
    char recvBuffer[MSG_TOTAL_LENGTH];

    while (!closeServer && recvResult > 0)
    {
        recvResult = recv(client.m_socket, recvBuffer, MSG_TOTAL_LENGTH, 0);
        if (recvResult > 0)
        {
            std::string str(MSG_TOTAL_LENGTH, MSG_NULL_CHAR);
            str.insert(0, recvBuffer, MSG_TOTAL_LENGTH);

            Message msg = Message(str);
            msg.Print();
        }
        /*else if (recvResult == 0)
        {
            coutMutex.lock();
            std::cout << "connection closing" << std::endl;
            coutMutex.unlock();
        }*/
        else
        {
            closesocket(client.m_socket);

            allClientsMutex.lock();
            client.m_active = false;
            allClientsMutex.unlock();

            coutMutex.lock();
            std::cout << "/ - / Client disconnected [" << (int)client.m_id << "]" << std::endl;
            coutMutex.unlock();
        }
    }

    return;
}

int main()
{
    WSAData wsaData;
    //wsaData.wVersion = MAKEWORD(2, 2);
    //wsaData.iMaxSockets = 2;
    //wsaData->iMaxUdpDg = 0;
    //wsaData->lpVendorInfo = 0;

    /* Initialize Winsock */
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        throw std::runtime_error("WSAStartup failed");
    }

    /* Obtain list of suitable interfaces */
    struct addrinfo hints = { 0 };
    hints.ai_family = AF_INET; /* IPv4 and IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_protocol = IPPROTO_TCP; /* TCP */
    hints.ai_flags = AI_PASSIVE; /* Allow accepting of connections */
    struct addrinfo* result = NULL;
    iResult = getaddrinfo(NULL, "8080", &hints, &result);
    if (iResult != 0)
    {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed with error");
    }

    /* Create a new socket */
    SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        WSACleanup();
        throw std::runtime_error("socket failed with error");
    }
    /* Bind on the obtained interface */
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        throw std::runtime_error("bind failed with error");
    }
    freeaddrinfo(result);


    /*
    Start a separate thread to listen for and accept incoming client sockets.
    This thread will create a new thread for each new client, which will listen for messages arriving from said client.
    */
    std::thread socketListenerThread = std::thread(AcceptClientSockets, listenSocket);

    /* Close server on demand */
    std::cin.get();

    /* Cleanup */
    closeServer = true;
    socketListenerThread.join();

    for (int i = 0; i < clientListenerThreads.size(); i++)
    {
        clientListenerThreads[i].join();
    }

    WSACleanup();

    return 0;
}

bool Send(Message message)
{
    /*int messageLength = m_outgoingQueue.front().size();
    const char* message = m_outgoingQueue.front().c_str();
    for (int i = 0; i < MSG_TOTAL_LENGTH; i++)
    {
        if (i < messageLength)
        {
            m_outgoing[i] = message[i];
        }
        else
        {
            m_outgoing[i] = 0;
        }
    }
    m_outgoing[MSG_TOTAL_LENGTH - 1] = MSG_NULL_CHAR;*/

    // send
    message.Print();
    int sendResult = send(allClients[(int)message.m_recipientID - 2].m_socket, message.m_raw.c_str(), MSG_TOTAL_LENGTH, 0);
    if (sendResult == 0)
    {
        return false;
        // connection closed
        //m_disconnected = true;
    }
    else if (sendResult == SOCKET_ERROR)
    {
        return false;
        // connection error
    }
    else
    {
        // send success
        return true;
        //m_outgoingQueue.erase(m_outgoingQueue.begin());
    }
}
