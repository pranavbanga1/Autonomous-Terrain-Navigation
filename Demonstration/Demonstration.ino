#include "Shield2AMotor.h"
#include <Servo.h>

// Initialize the motor
Shield2AMotor motor(SIGNED_MAGNITUDE);

// Initialize the servos
Servo baseServo;    // Servo for base and tilt
Servo gripperServo; // Servo for gripper

// Define the pins for ultrasonic sensors and IR sensors
const int trigPin1 = 3;  // Trigger pin for left ultrasonic sensor
const int echoPin1 = 2;  // Echo pin for left ultrasonic sensor

const int trigPin2 = 5;  // Trigger pin for right ultrasonic sensor
const int echoPin2 = 4;  // Echo pin for right ultrasonic sensor

const int frontIRPin = A0  ; // Front IR sensor for dead ends and open paths (GP2Y0E02A)
const int boxIRPin = A1;    // Front IR sensor for box detection (GP2Y0E02A)

const int gp101Pin = 7;     // Pin for GP101 active/standby control

void setup()
{
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate

  // Attach servos
  baseServo.attach(9);     // Base servo connected to pin 9
  gripperServo.attach(8);  // Gripper servo connected to pin 8

  // Initialize ultrasonic sensors
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Initialize GP101 for IR sensor activation
  pinMode(gp101Pin, OUTPUT);
  digitalWrite(gp101Pin, HIGH);  // Activate the IR sensors

  // Initial positions
  baseServo.write(0);      // Start with base at 0 degrees
  gripperServo.write(70);  // Gripper initially closed at 70 degrees
  
  delay(1000);             // Delay for 1 second

  Serial.println("Setup Complete. Starting the loop...");
}

long readUltrasonicDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;  // Distance in cm
}

int readIRSensor(int pin)
{
  // Assuming GP2Y0E02A outputs analog values based on distance
 //int sensorValue = analogRead(pin);
  //return sensorValue;
}

void loop()
{
  long leftDistance = readUltrasonicDistance(trigPin1, echoPin1);
  long rightDistance = readUltrasonicDistance(trigPin2, echoPin2);
  int frontIR = readIRSensor(frontIRPin);
  int boxIR = readIRSensor(boxIRPin);

  // Display sensor readings on Serial Monitor
  Serial.print("Left Ultrasonic Distance: ");
  Serial.print(leftDistance);
  Serial.println(" cm");

  Serial.print("Right Ultrasonic Distance: ");
  Serial.print(rightDistance);
  Serial.println(" cm");

  Serial.print("Front IR Sensor Value: ");
  Serial.println(frontIR);

  Serial.print("Box IR Sensor Value: ");
  Serial.println(boxIR);

  // Wall following logic
  if (frontIR >! 600) // Front IR detects a dead end or obstacle (adjust value based on testing)
  {
    Serial.println("Obstacle ahead! Turning left...");
    motor.control(-100, 100);  // Rotate left
    delay(500);                // Adjust time for proper turning
  }
  else if (leftDistance < 20) // Too close to the left wall
  {
    Serial.println("Too close to the left wall. Adjusting right...");
    motor.control(100, 50);  // Move right slightly
  }
  else if (rightDistance < 20) // Too close to the right wall
  {
    Serial.println("Too close to the right wall. Adjusting left...");
    motor.control(50, 100);  // Move left slightly
  }
  else // If no close walls
  {
    Serial.println("No obstacles, moving forward...");
    motor.control(100, 100); // Move straight
  }

/*  // Box detection logic
  if (boxIR > 600) // IR sensor detects a box (adjust value based on testing)
  {
    Serial.println("Box detected! Gripper action initiated.");
    
    // Open gripper to pick up package
    gripperServo.write(20);  // Gripper opens at 20 degrees
    delay(2000);             // Wait 2 seconds

    // Close the gripper after picking up the package
    gripperServo.write(70);  // Close gripper to hold package
    delay(2000);             // Wait 2 seconds

    // Move the base up to 110 degrees after picking up the package
    baseServo.write(110);    // Tilt base to 110 degrees
    delay(2000);             // Wait 2 seconds
  }
  else
  {
    // Gripper stays closed
    gripperServo.write(70);  // Keep gripper closed
  } */

  delay(1000);  // Add delay to reduce data overload in the Serial Monitor
}
