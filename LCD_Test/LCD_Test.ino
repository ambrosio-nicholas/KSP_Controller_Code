#include <Wire.h>
#include "KerbalSimpit.h"
#include <LiquidCrystal_I2C.h>

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);
// Create a new Altitude struct
altitudeMessage myAltitude;

// Common I2C addresses:
// 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte block[8] = {
  B00000,B00000,B11111,B11111,B11111,B11111,B00000,B00000
};

void setup() {
  lcd.init();          // initialize LCD
  lcd.backlight();     // turn on backlight

  lcd.createChar(0, block);

  // Open the serial connection.
  Serial.begin(115200);

  // Set up the build in LED, and turn it on.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message in KSP to indicate handshaking is complete.
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.
  mySimpit.inboundHandler(messageHandler);
  // Send a message to the plugin registering for the Altitude channel.
  // The plugin will now regularly send Altitude messages while the
  // flight scene is active in-game.
  mySimpit.registerChannel(ALTITUDE_MESSAGE);

  lcd.setCursor(0, 0);
  lcd.print("Altitude: ");

  for (int i = 0; i < 3; i++){
    lcd.write(byte(0));
  }
}

void loop() {
  mySimpit.update();
  lcd.setCursor(10, 0);
  lcd.print(myAltitude.sealevel);
  delay(5);
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case ALTITUDE_MESSAGE:
    // Checking if the message is the size we expect is a very basic
    // way to confirm if the message was received properly.
    if (msgSize == sizeof(altitudeMessage)) {
      // Convert the message we received to an Altitude struct.
      myAltitude = parseMessage<altitudeMessage>(msg);
    }
    break;
  }
}