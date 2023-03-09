/* Servo Controller for

    Motion Mind Rev 2 HW
    solutions-cubed.com
    https://www.solutions-cubed.com/content/Downloads/MotionMind3Docs/MOTM3DS_8.pdf

    DC Motor Controller

    Used as a linear actuator in:
    Open Loop Mode

    Using Analog Input for position (or velocity?) control




*/

#define VELOCITY_CTRL_PIN 10
#define DIRECTION_PIN 7
#define BRAKE_PIN 4




int pos = 0;    // variable to store the servo position

int velocity = 0;  // Store the servo position

bool extend = false;   // when true, extends, false retracts

#define EXTEND false
#define RETRACT true



void setVelocity(int16_t velocity) {
  // velocity = velocity + 90;
  analogWrite(VELOCITY_CTRL_PIN, velocity);
}

void setDirection(bool extension) {
  if (extension) {
    digitalWrite(DIRECTION_PIN, LOW);
  } else {
    digitalWrite(DIRECTION_PIN, HIGH);
  }
}

void applyBrake(bool brakeState) {
  if (brakeState) {
    digitalWrite(BRAKE_PIN, LOW);
  } else {
    digitalWrite(BRAKE_PIN, HIGH);
  }
}

// As the actuator has safe limit switches built in, an easy way to ensure it is at zero position
// is just run {max speed}? (Does it have to be max speed) in reverse for > than the amount of time this would take to complete
//based on experimentation

//  1m42s @ Velocity -20
//  0m52s @ Velocity 40
//  0m44s @ Velocity -90

void setPositionZero() {
  setDirection(RETRACT);
  setVelocity(40);
  // delay(60000);     // Delay for  minute to ensure servo has reached zero position
  delay(2000);
  setVelocity(0);
}


void setup() {
  pinMode(VELOCITY_CTRL_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(BRAKE_PIN, OUTPUT);
  applyBrake(false);
  setDirection(RETRACT);
  setPositionZero();
  // applyBrake(true);
  delay(1000);
}



void loop() {
  // applyBrake(false);
  setDirection(EXTEND);
  setVelocity(30);
  delay(10000);
  setDirection(RETRACT);
  setVelocity(50);
  delay(10000);
  // applyBrake(true);
}
