// ESP32 Code (on Robot)

#include <WiFi.h>
#include <WiFiClient.h>

// WiFi Constants
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Server
WiFiServer server(80);

void setup() {
    // Initialize Serial for communication with Arduino Nano
    Serial.begin(115200);
    
    // Connect to WiFi
    connectToWiFi();
    
    // Start the server
    server.begin();
}

void loop() {
    // Check for incoming client
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        // Process the request
        if (request.indexOf("/data") != -1) {
            // Read data from Arduino Nano
            if (Serial.available() > 0) {
                String data = Serial.readStringUntil('\n');
                client.print(data);
            }
        } else if (request.indexOf("/control?") != -1) {
            // Parse and send control commands to Arduino Nano
            String xValue = getValue(request, "x");
            String yValue = getValue(request, "y");
            String buttonState = getValue(request, "button");
            
            Serial.print("X:");
            Serial.print(xValue);
            Serial.print("Y:");
            Serial.print(yValue);
            Serial.print("B:");
            Serial.println(buttonState);
        }

        client.stop();
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

String getValue(String data, String key) {
    int start = data.indexOf(key + "=") + key.length() + 1;
    int end = data.indexOf("&", start);
    if (end == -1) {
        end = data.length();
    }
    return data.substring(start, end);
}
