#define outputA 2
#define outputB 3
#define buttonPin 4

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int aState;
int aLastState;
unsigned long previousMillis = 0;
const long interval = 1000;
int seconds = 0;
int minutes = 0;
int hours = 0;
bool showClock = true;
bool buttonState;
bool lastButtonState = LOW;
int brightness = 0;

void setup() {
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  aLastState = digitalRead(outputA);

  myDisplay.begin();
  myDisplay.setIntensity(brightness);
  myDisplay.displayClear();

  myDisplay.displayText("Hello!", PA_CENTER, 30, 500, PA_OPENING_CURSOR, PA_CLOSING_CURSOR);
  while (!myDisplay.displayAnimate()) {
    // Wait until animation is done
  }
  myDisplay.displayClear();

  myDisplay.displayText("Made by Louis Gan", PA_CENTER, 28, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!myDisplay.displayAnimate()) {
    // Wait until animation is done
  }
}

void loop() {
  aState = digitalRead(outputA);
  buttonState = digitalRead(buttonPin);

  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      if (buttonState == LOW) {
        incrementBrightness();
      } else {
        incrementClock();
      }
    } else {
      if (buttonState == LOW) {
        decrementBrightness();
      } else {
        decrementClock();
      }
    }
    if (showClock) printClock();
  }
  aLastState = aState;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateClock();
    if (showClock) printClock();
  }

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      showClock = !showClock;
      if (!showClock) {
        myDisplay.displayText(("Sangjun Lee  "), PA_CENTER, 100, 1000, PA_SCROLL_LEFT, PA_SPRITE);
      } else {
        myDisplay.displayClear();
        printClock();
      }
    }
  }
  lastButtonState = buttonState;

  if (!showClock) {
    if (myDisplay.displayAnimate()) {
      myDisplay.displayReset();
    }
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
  seconds += 60;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 24) {
        hours = 0;
      }
    }
  }
}

void decrementClock() {
  if (seconds < 60) {
    seconds = 60;
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
  } else {
    seconds -= 30;
  }
}

void incrementBrightness() {
  brightness++;
  if (brightness > 15) brightness = 15;
  myDisplay.setIntensity(brightness);
}

void decrementBrightness() {
  brightness--;
  if (brightness < 0) brightness = 0;
  myDisplay.setIntensity(brightness);
}

void printClock() {
  char timeString[6];
  sprintf(timeString, "%02d%c%02d", hours, (seconds % 2 == 0) ? ':' : ' ', minutes);

  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.print(timeString);
}