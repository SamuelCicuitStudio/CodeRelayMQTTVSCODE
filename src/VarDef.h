#include <WiFi.h>               // Include WiFi library for ESP32
#include "SPIFFS.h"             // Include SPIFFS (SPI Flash File System) library
#include "PinsDef.h"            // Include Pins definition file
#include "HTML.h"               // Include HTML handling library
#include <WebServer.h>          // Include WebServer library for handling HTTP requests
#include <ArduinoJson.h>        // Include ArduinoJson library for JSON manipulation
#include <Arduino.h>            // Include Arduino core library
#include "FS.h"                 // Include FS library for file system operations
#include <PubSubClient.h>       // Include PubSubClient library for MQTT communication

/***************************************************************************************************************************************************************************************/
WiFiClient espClient;           // Create a WiFi client instance
PubSubClient client(espClient); // Create a MQTT PubSubClient instance using the WiFi client

/***************************************************************************************************************************************************************************************/
#define CONFIG_FILE "/Config.txt" // Define configuration file path in SPIFFS

/***************************************************************************************************************************************************************************************/
// Variables for WiFi settings
String wifiSSID = "VIVOFIBRA-5170";     // Default WiFi SSID
String wifiPassword = "99EF86FE52";     // Default WiFi password

String existingAPPassword = "12345678"; // Default existing AP password
String newAPPassword = "12345678";      // Default new AP password
String confirmAPPassword;               // Variable to confirm new AP password

/***************************************************************************************************************************************************************************************/
// Variables for Access Point (AP) settings
#define AP_SSID  "Config portal"       // AP SSID for configuration portal

/***************************************************************************************************************************************************************************************/
#define MQTT_TOPIC_1 "v1/devices/me/rpc/request/1"  // Default MQTT topic

// List of topics to subscribe to
const char* mqttSubscribeTopics[] = {
  "home/bedroom/switch1",
  "home/kitchen/temperature",
  "home/livingroom/humidity",
  // Add more topics as needed
};

/***************************************************************************************************************************************************************************************/
// Variables for MQTT settings
String mqttBroker = "192.168.86.34";    // Default MQTT broker address
String mqttUser = "MQTT_user";          // Default MQTT username
String mqttPassword = "plante_admin_@"; // Default MQTT password
int mqttPort = 1883;                    // Default MQTT port

/***************************************************************************************************************************************************************************************/
// Structure for relay control devices
struct DeviceSettings {
  String identifier;
  String name;
  String manufacturer;
  String model;
  String location;

  // Constructor with default values
  DeviceSettings() 
    : identifier("default_ID"), 
      name("default_Name"), 
      manufacturer("default_Manufacturer"), 
      model("default_Model"), 
      location("default_Location") {}
};

// Global variables for individual relay control devices
DeviceSettings relayDevice[4];

// Structure for sensor settings
struct SensorSettings {
  String identifier;
  String name;
  String manufacturer;
  String model;
  String location;

  // Constructor with default values
  SensorSettings() 
    : identifier("default_ID"), 
      name("default_Name"), 
      manufacturer("default_Manufacturer"), 
      model("default_Model"), 
      location("default_Location") {}
};

// Global variables for sensor settings (up to 50 sensors)
SensorSettings Sensor[50];

// Info variables
String sw_version = "1.0";               // Software version
String hw_version = "1.1";               // Hardware version
String configuration = "http://plante.ai"; // Configuration URL
uint32_t chipID;                        // ESP32 chip ID

/***************************************************************************************************************************************************************************************/
WebServer server(80);  // Create a WebServer instance on port 80
IPAddress ip(192, 168, 0, 1);       // Define IP address
IPAddress gateway(192, 168, 0, 1);  // Define gateway address
IPAddress subnet(255, 255, 255, 0); // Define subnet mask
