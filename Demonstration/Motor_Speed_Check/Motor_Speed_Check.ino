
#include "Shield2AMotor.h"
#include <Servo.h>

// Initialize the motor controller
Shield2AMotor motor(SIGNED_MAGNITUDE);

// Initialize the servos
Servo baseServo;    // Servo for base and tilt
Servo gripperServo; // Servo for gripper

// Variables for controlling servos and motors
int motorSpeedLeft = 0;
int motorSpeedRight = 0;
int baseServoAngle = 10;  // Default position for base servo
int gripperServoAngle = 50;  // Default position for gripper servo

void setup()
{
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Motor and Servo Test Started.");

  // Attach servos to pins
  baseServo.attach(13);      // Base servo on pin 9
  gripperServo.attach(12);   // Gripper servo on pin 8

  // Set initial positions of servos
  baseServo.write(baseServoAngle);   // Set base servo to default position
  gripperServo.write(gripperServoAngle); // Set gripper servo to default position

  Serial.println("Enter commands to control motors and servos:");
  Serial.println("Format: M L_SPEED R_SPEED for motors (e.g., M 100 100)");
  Serial.println("Format: S BASE_ANGLE GRIPPER_ANGLE for servos (e.g., S 90 45)");
}

void loop()
{
  // Check if there's data coming from the serial monitor
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read user input until a newline character

    // Parse the command for motor or servo control
    if (input.charAt(0) == 'M') {
      // Motor control: M L_SPEED R_SPEED
      int spaceIndex1 = input.indexOf(' ');
      int spaceIndex2 = input.indexOf(' ', spaceIndex1 + 1);

      if (spaceIndex1 > 0 && spaceIndex2 > 0) {
        motorSpeedLeft = input.substring(spaceIndex1 + 1, spaceIndex2).toInt();
        motorSpeedRight = input.substring(spaceIndex2 + 1).toInt();

        // Control the motors
        motor.control(motorSpeedLeft, motorSpeedRight);

        // Display the motor speeds
        Serial.print("Motor speeds set to - Left: ");
        Serial.print(motorSpeedLeft);
        Serial.print(" | Right: ");
        Serial.println(motorSpeedRight);
      }
    } else if (input.charAt(0) == 'S') {
      // Servo control: S BASE_ANGLE GRIPPER_ANGLE
      int spaceIndex1 = input.indexOf(' ');
      int spaceIndex2 = input.indexOf(' ', spaceIndex1 + 1);

      if (spaceIndex1 > 0 && spaceIndex2 > 0) {
        baseServoAngle = input.substring(spaceIndex1 + 1, spaceIndex2).toInt();
        gripperServoAngle = input.substring(spaceIndex2 + 1).toInt();

        // Control the servos
        baseServo.write(baseServoAngle);
        gripperServo.write(gripperServoAngle);

        // Display the servo angles
        Serial.print("Servos set to - Base Servo Angle: ");
        Serial.print(baseServoAngle);
        Serial.print(" | Gripper Servo Angle: ");
        Serial.println(gripperServoAngle);
      }
    }
  }
}
