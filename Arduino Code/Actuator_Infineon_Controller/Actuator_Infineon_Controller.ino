/* Infineon Motor Driver Analog & Direction to PWM Motor Controller

   Infineon Inputs



*/


// Infineon Motor Driver Pins



#define ENABLE_2 13
#define ENABLE_1 12
#define IN_2 11
#define IN_1 3

#define I_SENSE_1 A0
#define I_SENSE_2 A1


// Control Pins
#define BRAKE_INPUT 7
#define DIRECTION_INPUT 5

#define ANALOG_CONTROL_IN A5

#define RETRACT true
#define EXTEND false

#include <autoDelay.h>

autoDelay autoDelay;
#define DEBUG_DELAY_mS 500
#define DEBUG true

// Debug Messages
char extend_S[8] =  {"EXTEND "};
char retract_S[8] =  {"RETRACT"};
char true_S[8] = {"On"};
char false_S[8] = {"Off"};


// Global Variables
bool directionState = RETRACT;
bool brakeState = true;
uint16_t speedState = 0;

void setEnablePins(bool brake) {
  bool state;
  if (brake){
    state = false;
  } else {
    state = true;
  }
  digitalWrite(ENABLE_1, state);
  digitalWrite(ENABLE_2, state);
}

void stopPWM() {
  analogWrite(IN_2, 0);
  analogWrite(IN_1, 0);
}

void setPWMpins(bool direction, uint8_t speed) {
  if (direction) {
    analogWrite(IN_1, speed);
    analogWrite(IN_2, 0);
  } else {
    analogWrite(IN_1, 0);
    analogWrite(IN_2, speed);
  }
}

uint16_t sampleSpeed() {
  uint16_t  speedControl =  analogRead(ANALOG_CONTROL_IN);
  speedState = map(speedControl, 0, 1024, 0, 255);
  return speedState;
}


void sampleBrake() {
  brakeState = !digitalRead(BRAKE_INPUT);
}

void sampleDirection() {
  directionState = digitalRead(DIRECTION_INPUT);
}




void debugOutput(bool active) {
  if (active) {
    if (autoDelay.millisDelay(DEBUG_DELAY_mS)) {
      char brakeBuffer[8];
      char directionBuffer[8];
      if (!directionState) {                                   /// Check here for polarity of operation
        sprintf(directionBuffer, "%s", retract_S);
      } else {
        sprintf(directionBuffer, "%s", extend_S);
      }

      if (brakeState) {                                          // Check here for polarity of brake status
        sprintf(brakeBuffer, "%s", true_S);
      } else {
        sprintf(brakeBuffer, "%s", false_S);
      }
      char buffer[64];
      sprintf(buffer, "Brake: %s, Direction: %s, Speed: %i", brakeBuffer, directionBuffer, speedState);
      Serial.println(buffer);
    }
  }
}


void testMotorDrive() {
  Serial.println("Setting PWM");
  setPWMpins(EXTEND, 255);
  delay(2000);
  Serial.println("Setting Enable");
  setEnablePins(true);
  delay(5000);
  setPWMpins(RETRACT, 100);    // Changing direction
  delay(5000);
  Serial.println("Stopping movement");
  setPWMpins(true, 0);
  setEnablePins(false);
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Up");
  pinMode(ENABLE_2, OUTPUT);
  pinMode(ENABLE_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_1, OUTPUT);

  pinMode(BRAKE_INPUT, INPUT);
  pinMode(DIRECTION_INPUT, INPUT);

  setEnablePins(false);
  stopPWM();
}



void loop() {
  sampleBrake();
  sampleDirection();
  sampleSpeed();
  debugOutput(DEBUG);
  setEnablePins(brakeState);
  setPWMpins(directionState, speedState);

}
