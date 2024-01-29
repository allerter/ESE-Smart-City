#include <iostream>
#include <cstring>
#include <vector>
#include "function.h"

bool addPacketTX(
    std::vector<EndNode>& endNodes,
    std::vector<Message>& tx_buff,
    char location, std::string message
)
{
    short indexOfdestNode = -1;
    for(int i = 0; i < endNodes.size(); i++)
    {
        if(endNodes[i].location == location)
        {
            indexOfdestNode = i;
            break;
        }
    }

    //If no node was found return false
    if(indexOfdestNode == -1)
    {
        return false;
    }
    
    //Iterating over the buffer
    for(int i = 0; i < tx_buff.size(); i++)
    {
        //Continue if message in array contains content
        if(tx_buff[i].receiver != 0 || tx_buff[i].message != "")
        {
            continue;
        }

        //Clearing the message because it was sent successfully or exceeded number of allowed attempts
        tx_buff[i].receiver = endNodes[indexOfdestNode].i2c_address;
        tx_buff[i].message = message;

        //Returning true if message was put into the buffer successfully
        return true;
    }

    //Returning false otherwise
    return false;
}
