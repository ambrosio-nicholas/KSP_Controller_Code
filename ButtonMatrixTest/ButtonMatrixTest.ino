//Pins 53-41 top 5 are sense pins
byte buttonBitArray[5] = {};
const char* buttonText[40] = {"N/A","N/A","N/A","MNVR","NAV","FUEL","MAP","RCS","SAS","Anti-Target","Target","Radial-In","Radial-Out","AntiNormal","Normal","RetroGrade","Prograde","Maneuver","sasHold","AG9","AG8","AG7","AG6","AG5","AG4","AG3","AG2","AG1","AG0","Vel","Alt","CnclTW","-TW","+TW","CAM","Gear","Lights","Brakes","Abort","Stage"};

void setup() {
  Serial.begin(9600);

  //Set 5 pins as input
  for (int i = 53; i >= 48; i--){
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
  //Set 8 pins as output
  for (int i = 47; i >= 41; i--){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);//LOW);
  }
  pinMode(49, OUTPUT);
  digitalWrite(49, HIGH);
}

void loop() {
  //readButtons();
  //printButtons();
  //delay(500);
  if(digitalRead(53) || digitalRead(52) || digitalRead(51) || digitalRead(50) || digitalRead(48)){
    Serial.println(" BUTTON ");
  }
}

void readButtons(){
  int index = 0;
  //Set one column at a time high and check if each row is high or low
  for (int i = 40; i <= 47; i++){
    digitalWrite(i, HIGH);
    for(int j = 48; j <= 52; j++){
      //Check each sense pin and store their value in the bitArray
      bitWrite(buttonBitArray[index / 8], index % 8, digitalRead(j));
      if (digitalRead(j) == 1){
        Serial.println(i);
        Serial.print(" ON");
      }
      index++;
    }
    digitalWrite(i, LOW);
  }
}


void printButtons(){
   for (int button = 0; button < 40; button++) {
    // Is this button pressed?
    if (bitRead(buttonBitArray[button / 8], button % 8)) {

      Serial.println(buttonText[button]);

    }
    
  }
}
