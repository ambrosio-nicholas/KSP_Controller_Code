#include <AccelStepper.h>

// =========================
// Stepper Setup
// =========================

// 28BYJ-48 geared stepper
const long stepsPerRevolution = 4096;

// ULN2003 driver pin order
AccelStepper motor(
  AccelStepper::HALF4WIRE,
  2, 4, 3, 5
);

// =========================
// Setup
// =========================

void setup() {

  Serial.begin(115200);

  Serial.println("Stepper Angle Test");
  Serial.println("Enter degrees to move:");

  // Motion settings
  motor.setMaxSpeed(600);
  motor.setAcceleration(300);

  motor.setCurrentPosition(0);
}

// =========================
// Main Loop
// =========================

void loop() {

  // Keep motor moving smoothly
  motor.run();

  // Check for serial input
  if (Serial.available()) {

    // Read a line from serial
    String input = Serial.readStringUntil('\n');

    // Convert to float
    float degrees = input.toFloat();

    // Convert degrees -> steps
    long steps = degreesToSteps(degrees);

    // Relative move
    motor.move(steps);

    Serial.print("Moving ");
    Serial.print(degrees);
    Serial.print(" degrees (");
    Serial.print(steps);
    Serial.println(" steps)");
  }
}

// =========================
// Degree Conversion
// =========================

long degreesToSteps(float degrees) {

  return lround((degrees / 360.0) * stepsPerRevolution);
}