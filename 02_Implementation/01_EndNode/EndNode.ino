#include <Wire.h>

#define SLAVE_ADDRESS 20
#define STRING_BUFF_SIZE 10

int recivedSignal = 0;
int redPin = 13;
int yellowPin = 8;
int greenPin = 5;
int pirPin = 3;
int buttonPin = 4;
int lightsPin = 2;


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
  else if(msg == "lon") // Lights ON
  {
    digitalWrite(lightsPin, HIGH);
  }
  else if(msg == "loff") // Lights OFF
  {
    digitalWrite(lightsPin, LOW);
  }

}

void sendRequestedSignals(){
  const char* emergencySignalChar = digitalRead(buttonPin) ? "1" : "0";
  const char* lightsSignalChar = digitalRead(pirPin) ? "1": "0";
  Wire.write(emergencySignalChar);
  Wire.write(lightsSignalChar);
}

void setup() {
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(lightsPin, OUTPUT);

  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendRequestedSignals);
}


void loop() {
  delay(1000);
}
