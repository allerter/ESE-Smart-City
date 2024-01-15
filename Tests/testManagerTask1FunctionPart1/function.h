#include <iostream>
#include <vector>

#ifndef FUNCTION_H
#define FUNCTION_H

class Message {
public:
    uint8_t receiver;
    std::string message;
    uint8_t testResponseInteger;
    uint8_t tx_attempts;

    Message(uint8_t recv, const std::string& msg, uint8_t respInt)
        : receiver(recv), message(msg), testResponseInteger(respInt), tx_attempts(0) {}
};

// Communication Exception
class ComException : public std::runtime_error {
public:
    ComException(const std::string& message) : std::runtime_error(message) {}
};


// Function prototype
void task1_communication_part1(std::vector<Message>& tx_buff);

#endif  // FUNCTIONS_H