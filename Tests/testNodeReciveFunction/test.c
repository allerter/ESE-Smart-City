#include <stdio.h> 
#include <assert.h>
#include "function.h"

void testOffMessage(){
  printf("Running testOffMessage: ");
  fflush(stdout);
  char *msg = "off";
  struct Lights l;
  l.redLight = true;
  l.yellowLight = true;
  l.greenLight = true;
  l.streetLights = true;
  receiveEvent(msg, &l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  printf("PASSED\n");
}

void testRedMessage(){
  printf("Running testRedMessage: ");
  fflush(stdout);
  char *msg = "r";
  struct Lights l;
  l.redLight = true;
  l.yellowLight = true;
  l.greenLight = true;
  l.streetLights = true;
  receiveEvent(msg, &l);
  assert(l.redLight == true);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  printf("PASSED\n");
}

void testYellowMessage(){
  printf("Running testYellowMessage: ");
  fflush(stdout);
  char *msg = "y";
  struct Lights l;
  l.redLight = true;
  l.yellowLight = true;
  l.greenLight = true;
  l.streetLights = true;
  receiveEvent(msg, &l);
  assert(l.redLight == false);
  assert(l.yellowLight == true);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  printf("PASSED\n");

}

void testGreenMessage(){
  printf("Running testGreenMessage: ");
  fflush(stdout);
  char *msg = "g";
  struct Lights l;
  l.redLight = true;
  l.yellowLight = true;
  l.greenLight = true;
  l.streetLights = true;
  receiveEvent(msg, &l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == true);
  assert(l.streetLights == true);
  printf("PASSED\n");
}

void testYellowRedMessage(){
  printf("Running testYellowRedMessage: ");
  fflush(stdout);
  char *msg = "r_y";
  struct Lights l;
  l.redLight = true;
  l.yellowLight = true;
  l.greenLight = true;
  l.streetLights = true;
  receiveEvent(msg, &l);
  assert(l.redLight == true);
  assert(l.yellowLight == true);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  printf("PASSED\n");
}

void testLightsOnMessage(){
  printf("Running testLightsOnMessage: ");
  fflush(stdout);
  char *msg = "lon";
  struct Lights l;
  l.redLight = false;
  l.yellowLight = false;
  l.greenLight = false;
  l.streetLights = false;
  receiveEvent(msg, &l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  printf("PASSED\n");
}

void testLightsOffMessage(){
  printf("Running testLightsOffMessage: ");
  fflush(stdout);
  char *msg = "loff";
  struct Lights l;
  l.redLight = false;
  l.yellowLight = false;
  l.greenLight = false;
  l.streetLights = true;
  receiveEvent(msg, &l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == false);
  printf("PASSED\n");
}

int main(){
  testOffMessage();
  testRedMessage();
  testYellowMessage();
  testGreenMessage();
  testYellowRedMessage();
  testLightsOnMessage();
  testLightsOffMessage();
  return 0;
}
