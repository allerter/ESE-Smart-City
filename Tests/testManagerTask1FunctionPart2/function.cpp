#include <iostream>
#include <vector>
#include <string>
#include "function.h"

void task1_communication_part2(std::vector<EndNode>& endNodes)
{
    // Polling if there are emergency vehicles approaching
    for (int i = 0; i < endNodes.size(); i++)
    {


        // Read in first char
        char emergencySignal = endNodes[i].testEmergencySignal;

        // Read in second char
        char lightsSignal = endNodes[i].testLightsSignal;

        // Write result to end node
        if (emergencySignal == '1')
        {
            endNodes[i].emergencyApproaching = true;
        }
        else if (emergencySignal == '0')
        {
            endNodes[i].emergencyApproaching = false;
        }
        else
        {
            throw ComException("Result of polling slave " + std::to_string(endNodes[i].i2c_address) + " returned forbidden value");
        }

        // Write result to end node
        if (lightsSignal == '1')
        {
            endNodes[i].citizenDetected = true;
        }
        else if (lightsSignal == '0')
        {
            endNodes[i].citizenDetected = false;
        }
        else
        {
            throw ComException("Result of polling slave " + std::to_string(endNodes[i].i2c_address) + " returned forbidden value");
        }
    }
}
