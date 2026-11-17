// ===== SELECT LED TO TEST =====
const byte TEST_ROW = 25;
const byte TEST_COL = 31;
// ===============================

void setup() {

  // Set all row pins as outputs
  for (byte pin = 22; pin <= 29; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // Set all column pins as outputs
  for (byte pin = 30; pin <= 34; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  
  //Turn everything off
  allOff();
}

void loop() {
  cycle();
  //singleTest(); 
  //delay(20); 
}

void cycle() {
  for (int j = 30; j <= 34; j++) {
    for (int i = 22; i <= 29; i++) {

      // Turn on this LED
      digitalWrite(j, LOW);
      digitalWrite(i, HIGH);

      // Keep it on for 10 micro-seconds
      delay(0.01);

      // Turn it off
      digitalWrite(j, HIGH);
      digitalWrite(i, LOW);

      // Keep it off for 10 micro-seconds
      delay(0.01);
    }
  }
}

void singleTest(){
  allOff();
  // Turn on the selected LED
  digitalWrite(TEST_ROW, HIGH);
  digitalWrite(TEST_COL, LOW);
}

void allOff(){
  for (int row = 22; row <= 29; row++)
  digitalWrite(row, LOW);

  for (int col = 30; col <= 34; col++)
    digitalWrite(col, HIGH);
}