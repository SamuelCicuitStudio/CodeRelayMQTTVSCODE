#include "Function.h"


void setup() {
  // Start Serial for debugging
  Serial.begin(115200);

  // Mount SPIFFS
  if (!mountSPIFFS()) {
    return;
  }
  // Create a configure the  config file
 //createConfigFile();
  // Load settings from config file
  loadSettings();

  // Connect to WiFi
  connectToWiFi();

  // Handle root endpoint
server.on("/",handleRoot);
server.on("/api/saveSettings", HTTP_POST,handleSaveSettings); // Changed to POST
server.on("/validateExistingPassword", HTTP_POST, handleSaveNewPassword); // Changed to POST
server.on("/api/reboot", HTTP_POST, handleRebootRequest); // Changed to POST
server.on("/api/getSensorData", HTTP_POST, [](){
    String sensorData = prepareSensorData(); // Call your function to prepare sensor data

    // Respond with JSON data
    server.send(200, "application/json", sensorData);
});


  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
 server.handleClient();
}
