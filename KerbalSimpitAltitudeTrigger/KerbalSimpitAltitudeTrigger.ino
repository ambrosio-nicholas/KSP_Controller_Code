#include "KerbalSimpit.h"

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);
// Create a new Altitude struct
altitudeMessage myAltitude;

const int segE  = 2;
const int segD  = 3;
const int segDP = 4;
const int segC  = 5;
const int segG  = 6;
const int digit4 = 7;
const int digit1 = 8;
const int segA  = 9;
const int segF  = 10;
const int digit2 = 11;
const int digit3 = 12;
const int segB  = 13;

const int digits[4] = {digit1, digit2, digit3, digit4};

// Segment order: A B C D E F G
const bool numbers[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  //Set up the 7-segments pins
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segDP, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

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
}

void loop() {
  // Check for new serial messages.
  mySimpit.update();
  if (myAltitude.sealevel != myAltitude.surface){
    displayNumber(myAltitude.surface);  
  }else{
    displayNumber(myAltitude.sealevel);
  }
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

void clearDigits() {
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
}

void setSegments(int num) {

  digitalWrite(segA, numbers[num][0]);
  digitalWrite(segB, numbers[num][1]);
  digitalWrite(segC, numbers[num][2]);
  digitalWrite(segD, numbers[num][3]);
  digitalWrite(segE, numbers[num][4]);
  digitalWrite(segF, numbers[num][5]);
  digitalWrite(segG, numbers[num][6]);
}

void showDigit(int num, int digitPin) {

  clearDigits();

  setSegments(num);

  // COMMON CATHODE
  digitalWrite(digitPin, LOW);

  delay(2);

  digitalWrite(digitPin, HIGH);
}

void displayNumber(int value) {

  int thousands = (value / 1000) % 10;
  int hundreds  = (value / 100) % 10;
  int tens      = (value / 10) % 10;
  int ones      = value % 10;

  showDigit(thousands, digit1);
  showDigit(hundreds,  digit2);
  showDigit(tens,      digit3);
  showDigit(ones,      digit4);
}
