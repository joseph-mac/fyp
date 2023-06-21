#include "NetworkDefs.h"

#include <iostream>

Message::Message(std::string rawString)
{
    PopulateFromRaw(rawString);
}

Message::Message(byte senderID, byte recipientID, std::string message)
{
    std::string msg(MSG_TOTAL_LENGTH, MSG_NULL_CHAR);

    // Header //
    msg[0] = senderID; // Sender
    msg[1] = recipientID; // Recipient

    // Data //
    if (message.length() < MSG_DATA_LENGTH)
    {
        msg.insert(2, message);
    }
    else
    {
        msg.insert(2, message.substr(0, MSG_DATA_LENGTH - 1));
    }

    PopulateFromRaw(msg);
}

void Message::Print()
{
    std::cout << "/.../ ";
    if (m_senderID == 0)
    {
        std::cout << "Server";
    }
    else if (m_senderID == 1)
    {
        std::cout << "All clients";
    }
    else
    {
        std::cout << "Client[" << (int)m_senderID << "]";
    }

    std::cout << " -> ";

    if (m_recipientID == 0)
    {
        std::cout << "Server";
    }
    else if (m_recipientID == 1)
    {
        std::cout << "All clients";
    }
    else
    {
        std::cout << "Client[" << (int)m_recipientID << "]";
    }
    std::cout << ": '" << m_data.c_str() << "'" << std::endl;
}

void Message::PopulateFromRaw(std::string rawString)
{
    m_raw = rawString;

    m_senderID = m_raw[0];
    m_recipientID = m_raw[1];

    m_data = m_raw.substr(2, MSG_DATA_LENGTH);
}
