#define outputA 2
#define outputB 3

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int aState;
int aLastState;  
unsigned long previousMillis = 0; // will store last time the clock was updated
const long interval = 1000; // interval at which to update clock (milliseconds)
int seconds = 0;
int minutes = 0;
int hours = 0;

void setup() { 
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  
  Serial.begin(9600);
  aLastState = digitalRead(outputA); 

  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();
} 

void loop() { 
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  if (aState != aLastState){     
    if (digitalRead(outputB) != aState) { 
      incrementClock();
    } else {
      decrementClock();
    }
    printClock();
  } 
  aLastState = aState; // Updates the previous state of the outputA with the current state
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateClock();
    printClock();
  }
}

void updateClock() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    incrementClock();
  }
}

void incrementClock() {
  minutes++;
  if (minutes >= 60) {
    minutes = 0;
    hours++;
    if (hours >= 24) {
      hours = 0;
    }
  }
}

void decrementClock() {
  if (minutes == 0) {
    minutes = 59;
    if (hours == 0) {
      hours = 23;
    } else {
      hours--;
    }
  } else {
    minutes--;
  }
}

void printClock() {
  char timeString[6];
  sprintf(timeString, "%02d:%02d", hours, minutes);

  Serial.println(timeString);
  
  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.print(timeString);
}