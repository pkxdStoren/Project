#include <FlexiTimer2.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
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

// WiFi Constants
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

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
    
    // Connect to WiFi
    connectToWiFi();
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
    
    // Transmit data via ESP8266
    transmitData(flameValue, distance, smokeValue, temperature, micValue);
    
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

void transmitData(int flame, long distance, int smoke, float temperature, int mic) {
    // Transmit data via ESP8266
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const char* host = "your_server_host";
        const int httpPort = 80;
        
        if (!client.connect(host, httpPort)) {
            Serial.println("Connection failed");
            return;
        }

        String url = "/update?flame=" + String(flame) +
                     "&distance=" + String(distance) +
                     "&smoke=" + String(smoke) +
                     "&temperature=" + String(temperature) +
                     "&mic=" + String(mic);
        
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        delay(10);
        
        while (client.available()) {
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }
    }
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
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
