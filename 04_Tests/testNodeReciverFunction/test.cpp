#include <stdio.h> 
#include <assert.h>
#include "function.h"

void testOffMessage(){
  std::cout << "Running testOffMessage: ";
  std::string msg = "off";
  Light l = {true, true, true, true};
  receiveEvent(msg, l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  std::cout << "PASSED \n";
}

void testRedMessage() {
  std::cout << "Running testRedMessage: ";
  std::string msg = "r";
  Light l = {true, true, true, true};
  receiveEvent(msg, l);
  assert(l.redLight == true);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  std::cout << "PASSED\n";
}

void testYellowMessage() {
  std::cout << "Running testYellowMessage: ";
  std::string msg = "y";
  Light l = {true, true, true, true};
  receiveEvent(msg, l);
  assert(l.redLight == false);
  assert(l.yellowLight == true);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  std::cout << "PASSED\n";
}

void testGreenMessage(){
  std::cout << "Running testGreenMessage: ";
  std::string msg = "g";
  Light l = {true, true, true, true};
  receiveEvent(msg, l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == true);
  assert(l.streetLights == true);
  std::cout << "PASSED\n";
}

void testYellowRedMessage(){
  std::cout << "Running testYellowRedMessage: ";
  std::string msg = "r_y";
  Light l = {true, true, true, true};
  receiveEvent(msg, l);
  assert(l.redLight == true);
  assert(l.yellowLight == true);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  std::cout << "PASSED\n";
}

void testLightsOnMessage(){
  std::cout << "Running testLightsOnMessage: ";
  std::string msg = "lon";
  Light l = {false, false, false, false};
  receiveEvent(msg, l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  std::cout << "PASSED\n";
}

void testLightsOffMessage(){
  std::cout << "Running testLightsOffMessage: ";
  std::string msg = "loff";
  Light l = {false, false, false, true};
  receiveEvent(msg, l);
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == false);
  std::cout << "PASSED\n";
}

void testInvalidValue(){
  std::cout << "Running testInvalidValue: ";
  std::string msg = "random";
  Light l = {false, false, false, true};
  try{
    receiveEvent(msg, l);
    throw std::runtime_error("Exception should've been raised");

  } catch(const std::exception& e){
    // all good
  }
  assert(l.redLight == false);
  assert(l.yellowLight == false);
  assert(l.greenLight == false);
  assert(l.streetLights == true);
  std::cout << "PASSED\n";
}

int main(){
  testOffMessage();
  testRedMessage();
  testYellowMessage();
  testGreenMessage();
  testYellowRedMessage();
  testLightsOnMessage();
  testLightsOffMessage();
  testInvalidValue();
  return 0;
}
