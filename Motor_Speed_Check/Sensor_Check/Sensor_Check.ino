// Define the pins for ultrasonic sensors
const int trigPin1 = 3;  // Trigger pin for left ultrasonic sensor
const int echoPin1 = 2;  // Echo pin for left ultrasonic sensor

const int trigPin2 = 5;  // Trigger pin for right ultrasonic sensor
const int echoPin2 = 4;  // Echo pin for right ultrasonic sensor

// Define the pins for IR sensors
const int frontIRPin1 = A0;  // First front IR sensor (GP2Y0E02A)
const int frontIRPin2 = A1;  // Second front IR sensor (GP2Y0E02A)

// Function to read the distance from an ultrasonic sensor
long readUltrasonicDistance(int trigPin, int echoPin)
{
  // Ensure the trigger pin is low for a short time to start
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // 10 microsecond pulse
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance based on the speed of sound
  long distance = duration * 0.034 / 2; // Distance in cm
  return distance;
}

// Function to read the value from an IR sensor
int readIRSensor(int pin)
{
  return analogRead(pin);  // Returns a value between 0 and 1023 (distance depends on sensor)
}

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Sensor Test Started");

  // Initialize ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop()
{
  // Read distances from ultrasonic sensors
  long leftUltrasonicDistance = readUltrasonicDistance(trigPin1, echoPin1);
  long rightUltrasonicDistance = readUltrasonicDistance(trigPin2, echoPin2);

  // Read values from IR sensors
  int frontIRValue1 = readIRSensor(frontIRPin1);
  int frontIRValue2 = readIRSensor(frontIRPin2);

  // Output the sensor values to the Serial Monitor
  Serial.print("Left Ultrasonic Distance: ");
  Serial.print(leftUltrasonicDistance);
  Serial.println(" cm");

  Serial.print("Right Ultrasonic Distance: ");
  Serial.print(rightUltrasonicDistance);
  Serial.println(" cm");

  Serial.print("Front IR Sensor 1 Value: ");
  Serial.println(frontIRValue1);

  Serial.print("Front IR Sensor 2 Value: ");
  Serial.println(frontIRValue2);

  // Small delay before the next reading to make the output easier to read
  delay(1000);  // Wait 1 second between readings
}
