#include <Wire.h>

#define SLAVE_ADDRESS 23

int recivedSignal = 0;
int redPin = 13;
int yellowPin = 8;
int greenPin = 5;
int pirPin = 3;
bool isEmergencyRecieved = false;

#define STRING_BUFF_SIZE 10
void receiveEvent(int bytes) {
  char receivedChars[STRING_BUFF_SIZE];
  byte stringIndex = 0;
  
  while (Wire.available()) {
    char receivedChar = Wire.read();
    
    if(stringIndex < STRING_BUFF_SIZE - 1)
    {
      receivedChars[stringIndex] = receivedChar;
      stringIndex++;
    }
  }
  receivedChars[stringIndex] = '\0';

  //Converting received chars to a string
  String msg = String(receivedChars);

  if(msg == "off")
  {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
  }
  else if(msg == "y")
  {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  else if(msg == "r")
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
  }
  else if(msg == "g")
  {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
  else if(msg == "r_y")
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
}

void sendEmergencySignalValue(){
  Wire.write(String(digitalRead(pirPin)).c_str());
}

void setup() {
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  pinMode(pirPin, INPUT);
  
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendEmergencySignalValue);
}


void loop() {
  delay(1000);
}
