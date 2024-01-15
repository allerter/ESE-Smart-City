#include <iostream>
#include <vector>

#ifndef FUNCTION_H
#define FUNCTION_H

// Struct for EndNodes
class EndNode {
public:
    char location;
    int i2c_address;
    bool emergencyApproaching;
    bool citizenDetected;

    EndNode(
        char location,
        int i2c_address
    ):
        i2c_address(i2c_address),
        location(location),
        emergencyApproaching(false),
        citizenDetected(false)
        {}
};


class Message {
public:
    uint8_t receiver;
    std::string message;
    uint8_t tx_attempts;

    Message(uint8_t recv, const std::string& msg)
        : receiver(recv), message(msg), tx_attempts(0) {}
};

bool addPacketTX(
    std::vector<EndNode>& endNodes,
    std::vector<Message>& tx_buff,
    char location, std::string message
);

#endif  // FUNCTIONS_H
