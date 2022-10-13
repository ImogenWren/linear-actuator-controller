/* Servo Controller for 
 *  
 *  Motion Mind Rev 2 HW 
 *  solutions-cubed.com 
 *  https://www.solutions-cubed.com/content/Downloads/MotionMind3Docs/MOTM3DS_8.pdf
 *  
 *  DC Motor Controller
 *  
 *  Used as a linear actuator in:
 *  Open Loop Mode
 *  
 *  Using Servo.h Libary
 *  
 *  
 *  In open loop mode, servo angle corresponds to velocity where: 
 *  - 0deg = Full Reverse
 *  - 90deg = 0
 *  - 180deg = Full Forward
 *   
 *   actuatorVelocity() will translate this to +-90
 *  
*/

#include <Servo.h>

Servo actuator;  // create servo object to control a servo




int pos = 0;    // variable to store the servo position



void setVelocity(int16_t velocity){
  velocity = velocity + 90;
  actuator.write(velocity);  
}

// As the actuator has safe limit switches built in, an easy way to ensure it is at zero position
// is just run {max speed}? (Does it have to be max speed) in reverse for > than the amount of time this would take to complete
//based on experimentation

//  1m42s @ Velocity -20
//  0m52s @ Velocity 40
//

void setPositionZero(){
  setVelocity(40);
 // delay(10000);  
}


void setup() {
  actuator.attach(13);  // attaches the servo on pin 9 to the servo object  
  setPositionZero();
 // actuatorZeroV();      // Set to zero velocity on startup
}



void loop() {
    //actuator.write(110);    
   // setVelocity(0);         
}


void actuatorZeroV(){
      actuator.write(90);              // Set servo velocity to zero
}
