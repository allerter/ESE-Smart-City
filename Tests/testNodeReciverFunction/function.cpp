#include <iostream>
#include <string.h> 
#include "function.h"  // Include the header file

// Function definition
void receiveEvent(std::string msg, Light &light) {
    if(msg == "off")
    {
        light.redLight = false;
        light.yellowLight = false;
        light.greenLight = false;
    }
    else if(msg == "y")
    {
        light.redLight = false;
        light.yellowLight = true;
        light.greenLight = false;
    }
    else if(msg == "r")
    {
        light.redLight = true;
        light.yellowLight = false;
        light.greenLight = false;
    }
    else if(msg == "g")
    {
        light.redLight = false;
        light.yellowLight = false;
        light.greenLight = true;
    }
    else if(msg == "r_y")
    {
        light.redLight = true;
        light.yellowLight = true;
        light.greenLight = false;
    }

    if(msg == "lon") // Lights ON
    {
        light.streetLights = true;
    }
    else if(msg == "loff") // Lights OFF
    {
        light.streetLights = false;
    }
}
