#ifndef _MYENGINE_NETWORKDEFS_H_
#define _MYENGINE_NETWORKDEFS_H_

#include <string>

typedef unsigned char byte;

const byte MSG_NULL_CHAR = 0;
const int MSG_TOTAL_LENGTH = 256;
const int MSG_HEADER_LENGTH = 2;
const int MSG_DATA_LENGTH = MSG_TOTAL_LENGTH - MSG_HEADER_LENGTH;

struct Message
{
    Message(std::string rawString);
    Message(byte senderID, byte recipientID, std::string message);

    /**
    * Print the message, in a readable format, to the console.
    **/
    void Print();

    byte m_senderID;
    byte m_recipientID;
    std::string m_data;
    std::string m_raw;

private:
    void PopulateFromRaw(std::string rawString);
};

#endif
