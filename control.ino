// Remote Control Code (ESP32)

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// WiFi Constants
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Joystick Pins
const int joystickX = A0;
const int joystickY = A1;
const int joystickButton = 2;

void setup() {
    // Initialize Serial
    Serial.begin(115200);
    
    // Initialize OLED
    if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    
    // Initialize WiFi
    connectToWiFi();
}

void loop() {
    // Read joystick values
    int xValue = analogRead(joystickX);
    int yValue = analogRead(joystickY);
    int buttonState = digitalRead(joystickButton);

    // Send joystick values to the robot
    sendJoystickValues(xValue, yValue, buttonState);

    // Request sensor data from the robot
    String sensorData = requestSensorData();
    displaySensorData(sensorData);

    delay(100);
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void sendJoystickValues(int x, int y, int button) {
    WiFiClient client;
    if (client.connect("robot_ip_address", 80)) {
        String url = "/control?x=" + String(x) + "&y=" + String(y) + "&button=" + String(button);
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: robot_ip_address\r\n" +
                     "Connection: close\r\n\r\n");
        client.stop();
    }
}

String requestSensorData() {
    WiFiClient client;
    if (client.connect("robot_ip_address", 80)) {
        client.print(String("GET /data HTTP/1.1\r\n") +
                     "Host: robot_ip_address\r\n" +
                     "Connection: close\r\n\r\n");
        String line = client.readStringUntil('\n');
        client.stop();
        return line;
    }
    return "";
}

void displaySensorData(String data) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Sensor Data:");
    display.println(data);
    display.display();
}
