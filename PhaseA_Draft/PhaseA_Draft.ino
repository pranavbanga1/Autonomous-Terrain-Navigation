                                             #include "Shield2AMotor.h"
#include <Servo.h>

// Initialize the motor
Shield2AMotor motor(SIGNED_MAGNITUDE);

// Initialize the servos
Servo baseServo;    // Servo for base and tilt
Servo gripperServo; // Servo for gripper

void setup()
{
  // Attach servos
  baseServo.attach(9);     // Base servo connected to pin 8 ?//Switched the grper aqnd base
  gripperServo.attach(8);  // Gripper servo connected to pin 9

  // Initial positions
  baseServo.write(0);      // Start with base at 0 degrees
  gripperServo.write(70);  // Gripper initially closed at 70 degrees
  
  delay(1000);             // Delay for 1 second
}

void loop()
{
  // Open gripper to pick up package
  /*/gripperServo.write(20);  // Gripper opens at 20 degrees
  delay(2000);             // Wait 2 seconds

  // Close the gripper after picking up the package
 // gripperServo.write(70);  // Close gripper to hold package
  delay(2000);             // Wait 2 seconds

  // Move the base up to 110 degrees after picking up the package
  baseServo.write(110);    // Tilt base to 110 degrees
  delay(2000);             // Wait 2 seconds*/

  // Move both motors forward for 5 seconds
  motor.control(80, 100); // Full speed forward
  delay(3000); // Move forward for 5 seconds

 // Stop the motors1
  motor.control(0, 0);  // Stop motor 2
  delay(750); // Small delay before dropping
  
// Move both motors forward for 5 seconds
  motor.control(80, 100); // Full speed forward
  delay(4000); // Move forward for 5 seconds

  // Move both motors forward for 5 seconds
  motor.control(80, 100); // Full speed forward
  delay(3000); // Move forward for 5 seconds
  
  
  // Move forward for another 5 seconds
  //motor.control(200, 200); // Full speed forward again
 // motor.control(200, -250); // MOveRight
 // delay(2750); // Move forward for another 5 seconds 
  
  
/*
  // Move forward for another 5 seconds
  motor.control(200, 200); // Full speed forward again
  motor.control(200, 200); // Full speed forward again
  delay(5000); // Move forward for another 5 seconds
*/
  // Stop the motors1
  motor.control(0, 0);  // Stop motor 2
  delay(1000); // Small delay before dropping


          // Wait 2 seconds

 /* // Move the base up to 110 degrees after picking up the package
  baseServo.write(0);    // Tilt base to 110 degrees
  delay(2000);
   // Close the gripper after picking up the package
  gripperServo.write(20);  // Close gripper to hold package
  delay(2000);   */
  // Stop the loop to prevent continuous running
  while(true);
}
