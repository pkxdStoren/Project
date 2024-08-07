#include <ESP8266WiFi.h>
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
const int joyX = A0;
const int joyY = A1;
const int buttonPin = 2;

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);
    
    // Initialize OLED Display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    display.clearDisplay();
    display.display();
    
    // Initialize Joystick
    pinMode(buttonPin, INPUT_PULLUP);
    
    // Connect to WiFi
    connectToWiFi();
}

void loop() {
    // Read Joystick Values
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);
    int buttonState = digitalRead(buttonPin);
    
    // Send Joystick Values to Robot
    sendJoystickValues(xValue, yValue, buttonState);
    
    // Receive Data from Robot
    receiveRobotData();
    
    // Add delay for stability
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
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const char* host = "robot_ip_address";  // Replace with robot's IP address
        const int httpPort = 80;
        
        if (!client.connect(host, httpPort)) {
            Serial.println("Connection failed");
            return;
        }

        String url = "/control?x=" + String(x) + "&y=" + String(y) + "&button=" + String(button);
        
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        delay(10);
    }
}

void receiveRobotData() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        const char* host = "robot_ip_address";  // Replace with robot's IP address
        const int httpPort = 80;
        
        if (!client.connect(host, httpPort)) {
            Serial.println("Connection failed");
            return;
        }

        client.print(String("GET /data HTTP/1.1\r\n") +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        delay(10);
        
        String data = "";
        while (client.available()) {
            data += client.readStringUntil('\r');
        }
        displayData(data);
    }
}

void displayData(String data) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(data);
    display.display();
}
