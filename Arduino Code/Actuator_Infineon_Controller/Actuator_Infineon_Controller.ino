/* Infineon Motor Driver Controller - Speed, Direction & Brake Input

Program to drive Infineon Motor Driver Sheild for Arduino Uno
 
 Control Inputs:
 Brake - Digital (HIGH/LOW)
 Direction - Digital (HIGH/LOW)
 Speed - Analog (0 - 5v)

Outputs:
PWM - Controls Speed and Direction based on input to Infineon Motor Driver Shield
Enable - Digital pins (HIGH/LOW) to enable motor drive. 
*/


// Infineon Motor Driver Pins
#define ENABLE_2 13
#define ENABLE_1 12
#define IN_2 11
#define IN_1 3

// Current Sense Pins (Unused - but connected so do not use for other purposes)
#define I_SENSE_1 A0
#define I_SENSE_2 A1


// Control Pins - Connections from control device to these pins on Arduino
#define BRAKE_INPUT 7
#define DIRECTION_INPUT 5

#define ANALOG_CONTROL_IN A5

// Definitions just to make code more human readable
#define RETRACT true
#define EXTEND false

// This is my owm library, find it at https://github.com/PanGalacticTech/autoDelay_Library
// Only used in debug messages to time printing without blocking other software actions. Can be removed by removing (autoDelay.millisDelay(DEBUG_DELAY_mS)" 
// And replacing with millis() timer functions (see arduino example "blink without delay")
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
    state = false;                         // If brake action is reversed swap true and false here.
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

// Motor direction is controlled by writing PWM to one of two inputs and 0 to the other. 
// Potential for shoot through conditions! If operating long term should be written into a state machine with (0,0) (0,1) (1.0) as allowable states and (1,1) as 
// illegal state. State (0,1) cannot go direct to state (1,0), or vise versa without passing through state (0,0)
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
  brakeState = !digitalRead(BRAKE_INPUT);        // Brakestate is NOT -> BRAKE_INPUT level.  act
}                                                 // therefore brakeState is HIGH when pin is low - i.e. the system IS failsafe to brake active if the pin is low.
                                                   // active HIGH input to release software brake
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
