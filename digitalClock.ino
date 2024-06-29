#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();
}

void loop() {
  static unsigned long lastUpdate = 0;
  const unsigned long updateInterval = 1000;
  static int seconds = 0;
  static int minutes = 0;
  static int hours = 12;

  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    seconds++;
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

  char timeBuffer[6];
  sprintf(timeBuffer, "%02d:%02d", hours, minutes);

  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.print(timeBuffer);
  
  delay(1000);
}
