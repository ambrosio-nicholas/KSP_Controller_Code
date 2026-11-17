#include "KerbalSimpit.h"

KerbalSimpit mySimpit(Serial);

unsigned long lastSend = 0;

void setup() {

  //Serial.begin(9600);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  while (!mySimpit.init()) {
    delay(100);
  }

  digitalWrite(LED_BUILTIN, LOW);

  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);

  //mySimpit.registerChannel(THROTTLE_MESSAGE);
  
}

void loop() {

  mySimpit.update();

  // Only send every 50 ms (20 Hz)
  if (millis() - lastSend >= 50) {

    lastSend = millis();

    int throttle = analogRead(A0);

    //Serial.println(throttle);

    throttle = map(throttle, 120, 470, 0, 32767);

    // Only send if value changed meaningfully
    //if (abs(throttle - lastThrottle) > 100) {

    mySimpit.send(THROTTLE_MESSAGE, throttle);

    //}
  }
}