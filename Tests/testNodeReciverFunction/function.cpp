#include <iostream>
#include <string>

#include "function.h"

void receiveEvent(std::string msg, Light &light) {
    if (msg == "off") {
        light.redLight = false;
        light.yellowLight = false;
        light.greenLight = false;
    } else if (msg == "y") {
        light.redLight = false;
        light.yellowLight = true;
        light.greenLight = false;
    } else if (msg == "r") {
        light.redLight = true;
        light.yellowLight = false;
        light.greenLight = false;
    } else if (msg == "g") {
        light.redLight = false;
        light.yellowLight = false;
        light.greenLight = true;
    } else if (msg == "r_y") {
        light.redLight = true;
        light.yellowLight = true;
        light.greenLight = false;
    } else if (msg == "lon") {
        light.streetLights = true;
    } else if (msg == "loff") {
        light.streetLights = false;
    } else {
        // Handle unknown message
        throw std::runtime_error("Invalid value was givin");
    }
}
