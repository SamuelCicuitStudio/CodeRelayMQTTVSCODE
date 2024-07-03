#include <WiFi.h>
#include "SPIFFS.h"
#include "PinsDef.h"
#include "HTML.h"
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "FS.h"
#include <PubSubClient.h>
/***************************************************************************************************************************************************************************************/
WiFiClient espClient;
PubSubClient client(espClient);
/***************************************************************************************************************************************************************************************/
#define CONFIG_FILE "/Config.txt"

/***************************************************************************************************************************************************************************************/
// Variables for WiFi settings
String wifiSSID = "VIVOFIBRA-5170";
String wifiPassword = "99EF86FE52";

String existingAPPassword = "12345678";
String newAPPassword = "12345678";;
String confirmAPPassword;
/***************************************************************************************************************************************************************************************/
// Variables for Access Point (AP) settings
#define AP_SSID  "Config portal"
/***************************************************************************************************************************************************************************************/
#define MQTT_TOPIC_1 "v1/devices/me/rpc/request/1"
// List of topics to subscribe to
const char* mqttSubscribeTopics[] = {
  "home/bedroom/switch1",
  "home/kitchen/temperature",
  "home/livingroom/humidity",
  // Add more topics as needed
};

/***************************************************************************************************************************************************************************************/
// Variables for MQTT settings
String mqttBroker = "192.168.86.34";
String mqttUser = "MQTT_user";
String mqttPassword = "plante_admin_@";
int mqttPort = 1883;
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
  String identifier = "default";
  String name = "default";
  String manufacturer = "default";
  String model = "default";
  String location = "default";

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

  //Info
String sw_version = "1.0";
String hw_version = "1.1";
String configuration = "http://plante.ai";
uint32_t chipID;
/***************************************************************************************************************************************************************************************/
WebServer server(80);  // Create a WebServer instance
IPAddress ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

