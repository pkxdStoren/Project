// Arduino Nano Code

#include <FlexiTimer2.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Servo.h>

// Servo Constants
Servo servo[4][3];
const int servo_pin[4][3] = {
    {11, 12, 13},
    {2, 4, 7},
    {14, 15, 16},
    {8, 9, 10}
};

const float length_a = 50;
const float length_b = 77.1;
const float length_c = 27.5;
const float length_side = 71;
const float z_absolute = -28;

const float z_default = -50, z_up = -30;
const int threshold = 500; // Threshold value for flame detection

// Sensor Pins
const int flameSensorPin = A0;
const int trigPin = 5;
const int echoPin = 6;
const int smokeSensorPin = A1;
const int tempSensorPin = A2;
const int micPin = A3;

// BME280 Sensor
Adafruit_BME280 bme;

void setup() {
    // Attach servos
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            servo[i][j].attach(servo_pin[i][j]);
        }
    }
    initializePosition();
    
    // Initialize Serial Monitor
    Serial.begin(9600);
    
    // Initialize Sensors
    pinMode(flameSensorPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(smokeSensorPin, INPUT);
    pinMode(tempSensorPin, INPUT);
    pinMode(micPin, INPUT);

    // Initialize BME280
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
}

void loop() {
    // Read sensor values
    int flameValue = analogRead(flameSensorPin);
    long distance = measureDistance();
    int smokeValue = analogRead(smokeSensorPin);
    float temperature = bme.readTemperature();
    int micValue = analogRead(micPin);
    
    // Log sensor values
    logSensorValues(flameValue, distance, smokeValue, temperature, micValue);
    
    // Send data to ESP32
    sendDataToESP32(flameValue, distance, smokeValue, temperature, micValue);
    
    // Add any movement logic here
}

void initializePosition() {
    for (int i = 0; i < 4; i++) {
        moveLeg(i, z_default);
    }
}

void moveLeg(int leg, float z_position) {
    for (int j = 0; j < 3; j++) {
        servo[leg][j].writeMicroseconds(z_position);
    }
}

long measureDistance() {
    long duration;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    return (duration / 2) / 29.1;
}

void logSensorValues(int flame, long distance, int smoke, float temperature, int mic) {
    if (flame > threshold) {
        Serial.println("Flame detected!");
    }
    
    if (distance < 10) {
        Serial.println("Obstacle detected!");
    }
    
    Serial.print("Smoke Level: ");
    Serial.println(smoke);
    
    Serial.print("Temperature: ");
    Serial.println(temperature);
    
    Serial.print("Mic Value: ");
    Serial.println(mic);
}

void sendDataToESP32(int flame, long distance, int smoke, float temperature, int mic) {
    Serial.print(flame);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(",");
    Serial.print(smoke);
    Serial.print(",");
    Serial.print(temperature);
    Serial.print(",");
    Serial.println(mic);
}
