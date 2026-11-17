#include <LedControl.h>

// First pair: DIN, CLK, CS, number of displays
LedControl lc1(A4, A3, A5, 2);

// Second pair: DIN, CLK, CS, number of displays
LedControl lc2(10, 9, 11, 2);

void setup() {

  // Initialize first pair
  for (int i = 0; i < 2; i++) {
    lc1.shutdown(i, false);
    lc1.setIntensity(i, 0);
    lc1.clearDisplay(i);
  }

  delay(500);
  
  // Initialize second pair
  for (int i = 0; i < 2; i++) {
    lc2.shutdown(i, false);
    lc2.setIntensity(i, 0);
    lc2.clearDisplay(i);
  }

  delay(500);

  // Fill first pair
  for (int display = 0; display < 2; display++) {
    for (int digit = 0; digit < 8; digit++) {
      lc1.setDigit(display, digit, 8, false);
      delay(1000);
    }
  }

  // Fill second pair
  for (int display = 0; display < 2; display++) {
    for (int digit = 0; digit < 8; digit++) {
      lc2.setDigit(display, digit, 0, false);
      delay(1000);
    }
  }
}

void loop() {
}