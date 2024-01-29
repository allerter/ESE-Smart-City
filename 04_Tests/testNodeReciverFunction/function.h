#include <iostream>
#include <vector>

#ifndef FUNCTION_H
#define FUNCTION_H

class Light {
public:
  bool redLight;
  bool yellowLight;
  bool greenLight;
  bool streetLights;

  Light(
    bool redLight,
    bool yellowLight,
    bool greenLight,
    bool streetLights
  ):
    redLight(redLight),
    yellowLight(yellowLight),
    greenLight(greenLight),
    streetLights(streetLights)
    {}
  
};

// Function prototype
void receiveEvent(std::string msg, Light &light);

#endif  // FUNCTIONS_H
