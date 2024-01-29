#include <iostream>
#include <vector>

#ifndef FUNCTION_H
#define FUNCTION_H

// Struct for EndNodes
class EndNode {
public:
    int i2c_address;
    char location;
    bool emergencyApproaching;
    bool citizenDetected;
    char testEmergencySignal;
    char testLightsSignal;

    EndNode(
        int i2c_address,
        char location,
        bool emergencyApproaching,
        bool citizenDetected,
        char testEmergencySignal,
        char testLightsSignal
    ):
        i2c_address(i2c_address),
        location(location),
        emergencyApproaching(emergencyApproaching),
        citizenDetected(citizenDetected),
        testEmergencySignal(testEmergencySignal),
        testLightsSignal(testLightsSignal)
        {}
};


// Communication Exception
class ComException : public std::runtime_error {
public:
    ComException(const std::string& message) : std::runtime_error(message) {}
};

void task1_communication_part2(std::vector<EndNode>& endNodes);


#endif  // FUNCTIONS_H
