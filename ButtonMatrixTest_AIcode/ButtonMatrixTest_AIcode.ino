const uint8_t sensePins[5] = {53, 52, 51, 50, 48};
const uint8_t highPins[8]  = {49, 47, 45, 43, 41, 46, 44, 42};

// Stores the current and previous states of all 40 inputs (5 bytes * 8 bits)
byte buttonBitArray[5]   = {0, 0, 0, 0, 0};
byte previousBitArray[5] = {0, 0, 0, 0, 0};

// All 40 button names mapped in exact index order (0 to 39)
const char* buttonNames[40] = {
  // Byte 0 (Sense Pin 53)
  "Stage", "Abort", "Lights", "Brakes", "Gear", "CAM", "+", "-",
  // Byte 1 (Sense Pin 52)
  "TW_Cancel", "Alt", "Vel", "AG0", "AG1", "AG2", "AG3", "AG4",
  // Byte 2 (Sense Pin 51)
  "AG5", "AG6", "AG7", "AG8", "AG9", "SAS_Maintain", "SAS_Maneuver", "SAS_Prograde",
  // Byte 3 (Sense Pin 50)
  "SAS_Retrograde", "SAS_Normal", "SAS_Antinormal", "SAS_RadialIn", "SAS_RadialOut", "SAS_Target", "SAS_AntiTarget", "SAS",
  // Byte 4 (Sense Pin 48)
  "RCS", "MAP", "FUEL", "NAV", "MNVR", "noButton", "noButton", "noButton"
};

void setup() {
  Serial.begin(115200);

  // Set sense pins as inputs (requires external 10k pull-down resistors to GND)
  for (uint8_t i = 0; i < 5; i++) {
    pinMode(sensePins[i], INPUT);
  }

  // Set drive pins as outputs and start them LOW
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(highPins[i], OUTPUT);
    digitalWrite(highPins[i], LOW);
  }
}

void loop() {
  scanMatrix();
  checkButtonPresses();
  
  // Short delay for basic switch debouncing
  delay(15); 
}

void scanMatrix() {
  // Clear the current byte array before scanning
  for (uint8_t b = 0; b < 5; b++) {
    buttonBitArray[b] = 0;
  }

  // Drive one column HIGH at a time (bit 0 to 7)
  for (uint8_t bit = 0; bit < 8; bit++) {
    digitalWrite(highPins[bit], HIGH);
    delayMicroseconds(10); // Brief settle time for signal stability

    // Read all 5 sense pins (byte 0 to 4)
    for (uint8_t byteIdx = 0; byteIdx < 5; byteIdx++) {
      if (digitalRead(sensePins[byteIdx]) == HIGH) {
        // Pack the pressed state into the exact bit position using Bitwise OR
        buttonBitArray[byteIdx] |= (1 << bit);
      }
    }

    digitalWrite(highPins[bit], LOW);
  }
}

void checkButtonPresses() {
  for (uint8_t byteIdx = 0; byteIdx < 5; byteIdx++) {
    // Compare current byte against previous byte to find newly pressed bits (0 -> 1)
    byte newlyPressed = buttonBitArray[byteIdx] & ~previousBitArray[byteIdx];

    if (newlyPressed != 0) {
      for (uint8_t bit = 0; bit < 8; bit++) {
        if (newlyPressed & (1 << bit)) {
          uint8_t index = (byteIdx * 8) + bit;
          Serial.print("Pressed: ");
          Serial.println(buttonNames[index]);
        }
      }
    }

    // Save current state for the next loop
    previousBitArray[byteIdx] = buttonBitArray[byteIdx];
  }
}