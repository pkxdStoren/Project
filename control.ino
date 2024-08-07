// ESP32 code for remote control

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

// Robot IP Address
const char* robot_ip = "robot_ip_address"; // Replace with your robot's IP address

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
    if (client.connect(robot_ip, 80)) {
        String url = "/control?x=" + String(x) + "&y=" + String(y) + "&button=" + String(button);
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + robot_ip + "\r\n" +
                     "Connection: close\r\n\r\n");
        client.stop();
    }
}

String requestSensorData() {
    WiFiClient client;
    if (client.connect(robot_ip, 80)) {
        client.print(String("GET /data HTTP/1.1\r\n") +
                     "Host: " + robot_ip + "\r\n" +
                     "Connection: close\r\n\r\n");
        String data = client.readStringUntil('\n');
        client.stop();
        return data;
    }
    return "";
}

void displaySensorData(String data) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    
    // Split the data received
    int index1 = data.indexOf(',');
    int index2 = data.indexOf(',', index1 + 1);
    int index3 = data.indexOf(',', index2 + 1);
    int index4 = data.indexOf(',', index3 + 1);
    
    String flame = data.substring(0, index1);
    String distance = data.substring(index1 + 1, index2);
    String smoke = data.substring(index2 + 1, index3);
    String temperature = data.substring(index3 + 1, index4);
    String mic = data.substring(index4 + 1);
    
    // Display formatted data
    display.println("Sensor Data:");
    display.print("Flame: ");
    display.println(flame);
    display.print("Distance: ");
    display.println(distance);
    display.print("Smoke: ");
    display.println(smoke);
    display.print("Temp: ");
    display.println(temperature);
    display.print("Mic: ");
    display.println(mic);
    display.display();
}
