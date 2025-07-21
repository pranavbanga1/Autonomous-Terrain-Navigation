#include <PID_v1.h>
#include "Shield2AMotor.h"
#include <Servo.h>
// Initialize the motor controller and servos
Shield2AMotor motor(LOCKED_ANTI_PHASE);
Servo gripperServo; // Servo for gripper control
Servo baseServo; // Servo for base arm control
// Define pins for sensors
int count = 0;
const int trigPinRight = 3;
const int echoPinRight = 2;
const int obstacleIRPin = A2; // Top front IR sensor for obstacle detection
// Movement parameters
const int baseSpeed = 70; // Base speed for motors
const int targetDistance = 5; // Target distance from the right wall (in cm)
const int maxSensorRange = 100; // Maximum reliable ultrasonic sensor range
// PD control parameters
double Kp = 2.0, Kd = 1.0; // PD control gains
double error = 0, prevError = 0;
double correction = 0;
// Servo settings
const int gripperOpenAngle = 20;
const int gripperClosedAngle = 80;
const int basePickupAngle = 0; // Position for picking up the package
const int baseCarryAngle = 90; // Position to hold while moving
// Movement state variables
bool carryingPackage = false;
void setup() {
55
Serial.begin(9600);
pinMode(trigPinRight, OUTPUT);
pinMode(echoPinRight, INPUT);
pinMode(obstacleIRPin, INPUT);
gripperServo.attach(12);
baseServo.attach(13);
gripperServo.write(gripperClosedAngle); // Start with gripper closed
baseServo.write(baseCarryAngle); // Start with base at carry angle
Serial.println("Bot initialized and ready!");
motor.control(0, 0);
baseServo.write(basePickupAngle);
delay(500);
gripperServo.write(gripperOpenAngle);
delay(1000);
gripperServo.write(gripperClosedAngle);
delay(500);
baseServo.write(baseCarryAngle);
carryingPackage = true;
}
// Ultrasonic distance function
long readUltrasonicDistance(int trigPin, int echoPin) {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
long duration = pulseIn(echoPin, HIGH);
return duration * 0.034 / 2; // Convert duration to distance in cm
}
// Move forward while mitigating errors
void moveStraightWithErrorHandling() {
56
long rightDistance = readUltrasonicDistance(trigPinRight, echoPinRight);
// Validate sensor readings
if (rightDistance == 0 || rightDistance > maxSensorRange) rightDistance = targetDistance; //
Default to target
// Calculate error
error = targetDistance - rightDistance;
// PD correction
correction = Kp * error + Kd * (error - prevError);
// Compute motor speed signal for Locked Anti-Phase
int motorSpeed = baseSpeed + correction;
// Constrain motor speed
motorSpeed = constrain(motorSpeed, -255, 255);
// Debugging output for sensor readings and corrections
Serial.print("Right Distance: ");
Serial.print(rightDistance);
Serial.print(" cm | Correction: ");
Serial.println(correction);
// Control motors using Locked Anti-Phase
motor.control(motorSpeed, motorSpeed);
// Update previous error
prevError = error;
}
// Obstacle detection using top IR sensor
bool detectObstacleRight() {
int obstacleIRValue = analogRead(obstacleIRPin);
Serial.print("Top IR Value: ");
Serial.println(obstacleIRValue);
if (obstacleIRValue > 420) { // Threshold for obstacle detection
count++;
Serial.println("Obstacle detected! Turning right...");
motor.control(0, 0); // Stop the bot
57
delay(500);
turnRight();
return true;
}
return false;
}
// Obstacle detection using top IR sensor
bool detectObstacleLeft() {
int obstacleIRValue = analogRead(obstacleIRPin);
Serial.print("Top IR Value: ");
Serial.println(obstacleIRValue);
if (obstacleIRValue > 420) { // Threshold for obstacle detection
count++;
Serial.println("Obstacle detected! Turning right...");
motor.control(0, 0); // Stop the bot
delay(500);
turnLeft();
return true;
}
return false;
}
// Turn right
void turnRight() {
motor.control(-100, -100);
delay(100);
motor.control(150, -150); // Right turn with Locked Anti-Phase
delay(400); // Adjust delay for 90-degree turn
motor.control(0, 0); // Stop after turning
}
// Turn right
void turnLeft() {
58
motor.control(-100, -100);
delay(100);
motor.control(-150, 150); // Left turn with Locked Anti-Phase
delay(400); // Adjust delay for 90-degree turn
motor.control(0, 0); // Stop after turning
}
// Detect and pick up package
void detectAndPickUpPackage() {
int packageIRValue = analogRead(A2); // Package IR sensor (reuse A2 if needed)
Serial.print("Bottom IR Value (Package): ");
Serial.println(packageIRValue);
if (packageIRValue > 100 && !carryingPackage) {
Serial.println("Package detected! Picking up...");
motor.control(0, 0);
baseServo.write(basePickupAngle);
delay(500);
gripperServo.write(gripperOpenAngle);
delay(1000);
gripperServo.write(gripperClosedAngle);
delay(500);
baseServo.write(baseCarryAngle);
carryingPackage = true;
}
}
// Drop the package at the designated location
void dropPackage() {
if (carryingPackage) {
Serial.println("Dropping package...");
motor.control(0, 0); // Stop the robot
baseServo.write(basePickupAngle); // Lower the arm
delay(500);
59
gripperServo.write(gripperOpenAngle); // Open the gripper to release the package
delay(1000);
gripperServo.write(gripperClosedAngle); // Close the gripper
delay(500);
baseServo.write(baseCarryAngle); // Raise the arm to carry position
carryingPackage = false; // Update the package state
Serial.println("Package dropped!");
} else {
Serial.println("No package to drop.");
}
}
void loop() {
// Main logic
detectAndPickUpPackage();
while(!detectObstacleRight()){ // Check for obstacles
moveStraightWithErrorHandling(); // Move forward while mitigating errors
}
// Simulate drop-off point detection
if (count > 30) { // Example: Trigger drop after 30 iterations (average number of detections to the
drop location, noticed during tests)
dropPackage();
count = 0; // Reset count after dropping package
while(!carryingPackage){ //Pick up second package
while(!detectObstacleRight()){ // Check for obstacles
moveStraightWithErrorHandling(); // Move forward while mitigating errors
}
if(count > 10){
detectAndPickUpPackage();
}
}
// Simulate drop-off point detection
60
count = 0;
while(!detectObstacleLeft()){ // Check for obstacles going out of the maze
moveStraightWithErrorHandling(); // Move forward while mitigating errors
}
if (count > 20) { // Example: Trigger drop after 30 iterations
dropPackage(); //drop package at lobby
count = 0; // Reset count after dropping package
}
}
}
