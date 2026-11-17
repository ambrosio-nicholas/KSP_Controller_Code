#include <Servo.h>
#include "KerbalSimpit.h"

// =========================
// Kerbal Simpit
// =========================

KerbalSimpit mySimpit(Serial);
vesselPointingMessage myOrientation;

// =========================
// Stepper Setup
// =========================

const long stepsPerRevolution = 2048;

// Correct pin order for most ULN2003 boards
Servo pitchServo;


// =========================
// Setup
// =========================

void setup() {

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  while (!mySimpit.init()) {
    delay(100);
  }

  digitalWrite(LED_BUILTIN, LOW);

  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);

  mySimpit.inboundHandler(messageHandler);

  mySimpit.registerChannel(ROTATION_DATA_MESSAGE);

  // Conservative settings
  pitchServo.attach(2);
}

// =========================
// Main Loop
// =========================

void loop() {

  // Always process serial
  mySimpit.update();

  // Always run stepper
  pitchServo.write(myOrientation.pitch + 90);
  delay(20);
}

// =========================
// Receive KSP Messages
// =========================

void messageHandler(byte messageType, byte msg[], byte msgSize) {

  switch (messageType) {

    case ROTATION_DATA_MESSAGE:

      if (msgSize == sizeof(vesselPointingMessage)) {

        myOrientation =
          parseMessage<vesselPointingMessage>(msg);
      }

      break;
  }
}
