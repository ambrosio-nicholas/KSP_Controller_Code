// KSP CONTROLLER - BUTTON MATRIX & LED MATRIX

// ------------------------------------------------------------
// 1. BUTTON MATRIX CONFIGURATION
// ------------------------------------------------------------

const uint8_t sensePins[5] = {53, 52, 51, 50, 48};

const uint8_t highPins[8] = {
  49, 47, 45, 43, 41, 46, 44, 42
};

byte buttonBitArray[5] = {0, 0, 0, 0, 0};
byte ledBitArray[5] = {0, 0, 0, 0, 0};

const char* buttonNames[40] = {
  "Stage", "Abort", "Lights", "Brakes",
  "Gear", "CAM", "+", "-",

  "TW_Cancel", "Alt", "Vel", "AG0",
  "AG1", "AG2", "AG3", "AG4",

  "AG5", "AG6", "AG7", "AG8",
  "AG9", "SAS_Maintain", "SAS_Maneuver",
  "SAS_Prograde",

  "SAS_Retrograde", "SAS_Normal",
  "SAS_Antinormal", "SAS_RadialIn",
  "SAS_RadialOut", "SAS_Target",
  "SAS_AntiTarget", "SAS",

  "RCS", "MAP", "FUEL", "NAV",
  "MNVR", "noButton", "noButton", "noButton"
};

// ------------------------------------------------------------
// 2. LED MATRIX CONFIGURATION
// ------------------------------------------------------------

const uint8_t ledPositivePins[8] = {
  22, 23, 24, 25, 26, 27, 28, 29
};

const uint8_t ledNegativePins[5] = {
  34, 33, 32, 31, 30
};

// ------------------------------------------------------------
// 3. CONTROLLER STATES
// ------------------------------------------------------------

// Action Groups
uint8_t agLedIdx[10] = {0,1,2,4,3,5,7,6,22,23};

// Cycling displays
uint8_t velocityMode = 0;
uint8_t altitudeMode = 0;

// Exclusive display selection
// 0 = Maneuver
// 1 = Navigation
// 2 = Fuel
uint8_t displayMode = 0;

// SAS modes
// 0 = Maintain
// 1 = Maneuver
// 2 = Prograde
// 3 = Retrograde
// 4 = Normal
// 5 = Antinormal
// 6 = Radial In
// 7 = Radial Out
// 8 = Target
// 9 = Anti-Target
uint8_t sasMode = 0;

// Warning flags
bool lowSignal = false;
bool lowPower = false;
bool subOrbital = false;
bool overHeat = false;
bool atmosphere = false;

// ------------------------------------------------------------
// 4. SETUP
// ------------------------------------------------------------

void setup() {

  Serial.begin(115200);

  // Button sense pins
  for (uint8_t i = 0; i < 5; i++) {
    pinMode(sensePins[i], INPUT);
  }

  // Button drive pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(highPins[i], OUTPUT);
    digitalWrite(highPins[i], LOW);
  }

  // LED positive pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(ledPositivePins[i], OUTPUT);
    digitalWrite(ledPositivePins[i], LOW);
  }

  // LED negative pins
  for (uint8_t i = 0; i < 5; i++) {
    pinMode(ledNegativePins[i], OUTPUT);
    digitalWrite(ledNegativePins[i], HIGH);
  }

  Serial.println("KSP Controller Initialized");
}


// ------------------------------------------------------------
// 5. MAIN LOOP
// ------------------------------------------------------------

void loop() {

  scanMatrix();
  inputProcessing();
  scanLEDs();
}


// ------------------------------------------------------------
// 6. BUTTON MATRIX SCANNING                                       // The code is okay, need physical diodes on each switch to prevent cross-feed :(
// ------------------------------------------------------------

void scanMatrix() {

  for (uint8_t bit = 0; bit < 8; bit++) {
    digitalWrite(highPins[bit], HIGH);

    for (uint8_t byteIdx = 0; byteIdx < 5; byteIdx++) {
      bool isPressed = (digitalRead(sensePins[byteIdx]) == HIGH);
      bitWrite(buttonBitArray[byteIdx], bit, isPressed);

      /*if (isPressed) {
        Serial.println(buttonNames[byteIdx * 8 + bit]);
      }*/
    }

    digitalWrite(highPins[bit], LOW);
  }
}

// ------------------------------------------------------------
// 7. INPUT PROCESSING
// ------------------------------------------------------------

void inputProcessing(){
  /* Set the ledBitArray bits depending on what inputs were just recieved
     Note that the two arrays do not index nicely with each other...   */

  
  for (uint8_t i = 0; i <  10; i++)  {
    // The action group buttons (Idx: 11 -> 20)
    uint8_t buttonIndex = i + 11;
    bitWrite(ledBitArray[agLedIdx[i] / 8], agLedIdx[i] % 8, bitRead(buttonBitArray[buttonIndex / 8], buttonIndex % 8)); //Set the LED at the idx using the corresponding value from  the buttonIndex
  }

}

// ------------------------------------------------------------
// 8. LED MATRIX SCANNING
// ------------------------------------------------------------

void scanLEDs(){ // Only 1 LED will be on at any given moment, so they must cycle through very quickly to give a solid appearance

  for (uint8_t posIdx = 0; posIdx < 8; posIdx++) {
    for (uint8_t gndIdx = 0; gndIdx < 5; gndIdx++) {
      if (bitRead(ledBitArray[gndIdx], posIdx)){   // If the LED is supposed to be on, turn it on for this scan cycle 

        digitalWrite(ledPositivePins[posIdx], HIGH);
        digitalWrite(ledNegativePins[gndIdx], LOW);

        delay(0.01);

        digitalWrite(ledPositivePins[posIdx], LOW);
        digitalWrite(ledNegativePins[gndIdx], HIGH);

        delay(0.01);
      } else {
        delay(0.02);
      }
    }
  }
}