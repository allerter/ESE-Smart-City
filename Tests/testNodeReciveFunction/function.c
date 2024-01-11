#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include "function.h"  // Include the header file

// Function definition
void receiveEvent(char *msg, struct Lights *light) {
    if(strcmp(msg, "off") == 0)
    {
        light->redLight = false;
        light->yellowLight = false;
        light->greenLight = false;
    }
    else if(strcmp(msg, "y") == 0)
    {
        light->redLight = false;
        light->yellowLight = true;
        light->greenLight = false;
    }
    else if(strcmp(msg, "r") == 0)
    {
        light->redLight = true;
        light->yellowLight = false;
        light->greenLight = false;
    }
    else if(strcmp(msg, "g") == 0)
    {
        light->redLight = false;
        light->yellowLight = false;
        light->greenLight = true;
    }
    else if(strcmp(msg, "r_y") == 0)
    {
        light->redLight = true;
        light->yellowLight = true;
        light->greenLight = false;
    }

    if(strcmp(msg, "lon") == 0) // Lights ON
    {
        light->streetLights = true;
    }
    else if(strcmp(msg, "loff") == 0) // Lights OFF
    {
        light->streetLights = false;
    }
}
