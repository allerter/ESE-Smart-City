#include <stdbool.h>

#ifndef FUNCTION_H
#define FUNCTION_H

struct Lights {
  bool redLight;
  bool yellowLight;
  bool greenLight;
  bool streetLights;
};

// Function prototype
void receiveEvent(char *msg, struct Lights *light);

#endif  // FUNCTIONS_H
