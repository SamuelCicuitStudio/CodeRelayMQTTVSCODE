#include "VarDef.h"
void startAPMode();
void connectToWiFi();
void handleSaveNewPassword();
void handleRebootRequest();
void handleSaveSettings();
void saveSettings();
void saveSettings();
void loadSettings();
void handleRoot();
bool mountSPIFFS();
void createConfigFile();
String prepareSensorData();
void writeFile(fs::FS &fs, const char *path, const char *message);
String readFile(fs::FS &fs, const char *path) ;
void publishSensorConfig(uint8_t i, String name, String path, String type, const char* unit, const char* value_template,
                         String identifiers, String manufacturer, String model, String sw_version, String hw_version,
                         String configuration_url);
void subscribeTopic(String topic);
void publishTopics(String topic, String data);

bool reconnect() ;
bool checkConnection();
void sendCommand(uint8_t id, uint8_t cmd);
uint16_t CRC(uint8_t* bytes, uint8_t len);
void publishSwitchConfig(uint8_t i, const String& name, const String& path, const String& identifiers, 
                         const String& manufacturer, const String& model, const String& sw_version, 
                         const String& hw_version, const String& configuration_url);

void subscribeToAllTopics();
String getchipID();// get the chip id of the es32
/***************************************************************************************************************************************************************************************/
uint16_t CRC(uint8_t* bytes, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; ++i) {
      crc = (uint16_t)((crc & 0xFF00) | (crc & 0x00FF) ^ (bytes[i] & 0xFF));
      for (uint8_t j = 0; j < 8; j++) {
        if ((crc & 0x0001) > 0) {
          crc = (uint16_t)(crc >> 1);
          crc = (uint16_t)(crc ^ 0xA001);
        } else
          crc = (uint16_t)(crc >> 1);
      }
    }

    uint16_t buff = (uint16_t)(crc % 0x100);
    buff = (uint16_t)(buff << 8) & 0xFF00;
    buff |= (uint16_t)(crc / 0x100) & 0x00FF;
    return buff;
  }
/***************************************************************************************************************************************************************************************/
void sendCommand(uint8_t id, uint8_t cmd) {
    char buffer[256];
    uint8_t packet[8] = { id, 0x03, 0x00, cmd, 0x00, 0x01, 0x00, 0x00 };
    uint16_t CRC16 = CRC(packet, 6);
    packet[5 + 1] = (uint8_t)((CRC16 >> 8) & 0xFF);
    packet[5 + 2] = (uint8_t)(CRC16 & 0xFF);

    digitalWrite(DE, HIGH);
    digitalWrite(RE, HIGH);

    Serial2.write(packet, 8);
    Serial2.flush();

    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    delay(100);
    if (Serial2.available())
      Serial2.readBytes((char*)buffer, 7);
  }

/***************************************************************************************************************************************************************************************/
bool checkConnection() {
  if (!client.connected()) {
    return reconnect();
  }
  return true;
}
/***************************************************************************************************************************************************************************************/
bool reconnect() {
    const int numTopics = sizeof(mqttSubscribeTopics) / sizeof(mqttSubscribeTopics[0]);
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("PlantAI", mqttUser.c_str(), mqttPassword.c_str())) {
      Serial.println("connected");
      // Once connected, subscribe to the topics
      for (int i = 0; i < numTopics; ++i) {
        client.subscribe(mqttSubscribeTopics[i]);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  return client.connected();
}
/***************************************************************************************************************************************************************************************/
void subscribeToAllTopics() {
    const int numTopics = sizeof(mqttSubscribeTopics) / sizeof(mqttSubscribeTopics[0]);
  if (client.connected()) {
    for (int i = 0; i < numTopics; ++i) {
      client.subscribe(mqttSubscribeTopics[i]);
      Serial.print("Subscribed to: ");
      Serial.println(mqttSubscribeTopics[i]);
    }
  } else {
    Serial.println("MQTT not connected. Subscription failed.");
  }
}
/***************************************************************************************************************************************************************************************/
void subscribeTopic(String topic) {
  if (client.connected()) {
    client.subscribe(topic.c_str());
    Serial.print("Subscribed to: ");
    Serial.println(topic);
  } else {
    Serial.println("MQTT not connected. Subscription failed.");
  }
}
/***************************************************************************************************************************************************************************************/
void publishTopics(String topic, String data) {
  if (client.connected()) {
    client.publish(topic.c_str(), data.c_str());
    Serial.print("Published data: ");
    Serial.print(data);
    Serial.print(" to topic: ");
    Serial.println(topic);
  } else {
    Serial.println("MQTT not connected. Publish failed.");
  }
}
/***************************************************************************************************************************************************************************************/
void setBroker(String broker, int port, String user, String password) {
  mqttBroker = broker;
  mqttPort = port;
  mqttUser = user;
  mqttPassword = password;
  client.setServer(mqttBroker.c_str(), mqttPort);
}
/***************************************************************************************************************************************************************************************/
 void publishSwitchConfig(uint8_t i, const String& name, const String& path, const String& identifiers, 
                         const String& manufacturer, const String& model, const String& sw_version, 
                         const String& hw_version, const String& configuration_url) {
    // Create a JSON document with a static size
   DynamicJsonDocument doc(1024);

    // Populate the JSON document with the necessary fields
    doc["unique_id"] = "P" + String(chipID) + "S" + String(i);
    doc["name"] = name;
    doc["state_topic"] = "home/" + path + "/switch" + String(i);
    doc["command_topic"] = "home/" + path + "/switch" + String(i) + "/set";
    
    // Availability topic (assuming it's the same path with '/available' suffix)
    JsonArray availability = doc.createNestedArray("availability");
    JsonObject availabilityObj = availability.createNestedObject();
    availabilityObj["topic"] = "home/" + path + "/switch" + String(i) + "/available";
    
    // Payload and state fields
    doc["payload_on"] = "ON";
    doc["payload_off"] = "OFF";
    doc["state_on"] = "ON";
    doc["state_off"] = "OFF";
    doc["optimistic"] = false;
    doc["qos"] = 0;
    doc["retain"] = true;

    // Create a nested JSON object for device information
    JsonObject device = doc.createNestedObject("device");
    device["identifiers"] = identifiers;
    device["name"] = "Plante Plant switch " + String(i);
    device["manufacturer"] = manufacturer;
    device["model"] = model;
    device["sw_version"] = sw_version;
    device["hw_version"] = hw_version;
    device["configuration_url"] = configuration_url;

    // Serialize the JSON document into a buffer
    char buffer[1024];
    serializeJson(doc, buffer);

    // Construct the topic string
    String topic = "homeassistant/switch/plant/" + path + "/switch" + String(i) + "/config";

    // Publish the message to the MQTT broker
    client.publish(topic.c_str(), buffer, true);
}
/***************************************************************************************************************************************************************************************/
void publishSensorConfig(uint8_t i, String name, String path, String type, const char* unit, const char* value_template,
                         String identifiers, String manufacturer, String model, String sw_version, String hw_version,
                         String configuration_url) {
    DynamicJsonDocument doc(1024);

    doc["device_class"] = type;
    doc["name"] = name;
    doc["state_topic"] = "plant/" + path + "/block" + String(i) + type;
    doc["unit_of_measurement"] = unit;
    doc["value_template"] = value_template;
    doc["unique_id"] = "P" + String(chipID) + "S" + String(i);


    JsonObject device = doc.createNestedObject("device");
    device["identifiers"] = identifiers;
    device["name"] = "Plante Plant bloco " + String(i);
    device["manufacturer"] = manufacturer;
    device["model"] = model;
    device["sw_version"] = sw_version;
    device["hw_version"] = hw_version;
    device["configuration_url"] = configuration_url;

    char buffer[1024];
    serializeJson(doc, buffer);
    String topic = "homeassistant/sensor/plant/" + path + "/block" + String(i) + "/" + type + "/config";
    client.publish(topic.c_str(), buffer, true);
}

/***************************************************************************************************************************************************************************************/
String getchipID(){
  for (int i = 0; i < 17; i = i + 8) {
      chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    };

    return String("PlantAI-") + String(chipID);

}
/***************************************************************************************************************************************************************************************/
void startAPMode() {
  // Start the Access Point with static IP configuration
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(getchipID().c_str(), newAPPassword.c_str());
  Serial.println("Access Point mode started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}
/***************************************************************************************************************************************************************************************/
void connectToWiFi() {
  // Configure static IP address
  if (!WiFi.config(ip, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  // Begin WiFi connection
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  Serial.print("Connecting to WiFi");

  // Retry connecting to WiFi 20 times
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}
}
/***************************************************************************************************************************************************************************************/
void handleSaveNewPassword() {
    // Check if the request contains the required parameters
    if (server.hasArg("plain") && server.hasArg("currentPassword")) {
        String newPassword = server.arg("plain");
        String currentPassword = server.arg("currentPassword");

        // Check if the existing password matches
        if (currentPassword == existingAPPassword) {
            existingAPPassword = newPassword;
            server.send(200, "text/plain", "Password updated successfully.");
        } else {
            server.send(401, "text/plain", "Unauthorized: Incorrect current password.");
        }
    } else {
        server.send(400, "text/plain", "Bad Request: Missing new password or current password.");
    }
}
/***************************************************************************************************************************************************************************************/
void handleRebootRequest() {
  // Handle the reboot request here
  // For example, reset the ESP32
  Serial.println("Reboot Requested from Server!");
  Serial.println("Rebooting in 5 second.");
  delay(5000);
  esp_restart();
}
/***************************************************************************************************************************************************************************************/
void handleSaveSettings() {
    // Read the body of the request
    String payload = server.arg("plain");  // Assuming 'server' is your WebServer object

    // Deserialize JSON payload
    DynamicJsonDocument jsonBuffer(6144); // Adjusted size
    DeserializationError error = deserializeJson(jsonBuffer, payload);

    // Check for parsing errors
    if (error) {
        server.send(400, "application/json", "{\"error\": \"Failed to parse JSON\"}");
        return;
    }

    // Extract and update WiFi settings
    JsonObject wifi = jsonBuffer["wifi"];
    if (!wifi.isNull()) {
        wifiSSID = wifi["ssid"].as<String>();
        wifiPassword = wifi["password"].as<String>();
    }

    // Extract and update AP settings
    JsonObject apSettings = jsonBuffer["apSettings"];
    if (!apSettings.isNull()) {
        existingAPPassword = apSettings["existingPassword"].as<String>();
        newAPPassword = apSettings["newPassword"].as<String>();
        confirmAPPassword = apSettings["confirmNewPassword"].as<String>();
    }

    // Extract and update MQTT settings
    JsonObject mqtt = jsonBuffer["mqtt"];
    if (!mqtt.isNull()) {
        mqttBroker = mqtt["broker"].as<String>();
        mqttUser = mqtt["user"].as<String>();
        mqttPassword = mqtt["mqttpassword"].as<String>();
        mqttPort = mqtt["port"].as<int>();
    }

    // Extract and update relay control devices
    JsonArray devices = jsonBuffer["relayControl"]["devices"].as<JsonArray>();
    for (size_t i = 0; i < devices.size(); ++i) {
        JsonObject device = devices[i].as<JsonObject>();
        if (i < 4) {  // Ensure index is within bounds of the array
            relayDevice[i].identifier = device["identifier"].as<String>();
            relayDevice[i].name = device["name"].as<String>();
            relayDevice[i].manufacturer = device["manufacturer"].as<String>();
            relayDevice[i].model = device["model"].as<String>();
            relayDevice[i].location = device["location"].as<String>();
        }
    }

    // Extract and update sensor settings
    JsonArray sensorArray = jsonBuffer["sensors"].as<JsonArray>();
    for (size_t i = 0; i < sensorArray.size(); ++i) {
        JsonObject sensor = sensorArray[i].as<JsonObject>();
        if (i < 50) {  // Ensure index is within bounds of the array
            Sensor[i].identifier = sensor["identifier"].as<String>();
            Sensor[i].name = sensor["name"].as<String>();
            Sensor[i].manufacturer = sensor["manufacturer"].as<String>();
            Sensor[i].model = sensor["model"].as<String>();
            Sensor[i].location = sensor["location"].as<String>();
        }
    }

    // Save settings to persistent storage or EEPROM
    saveSettings();  // Implement your saveSettings() function accordingly

    // Respond with success message
    server.send(200, "application/json", "{\"message\": \"Settings saved successfully\"}");
}
/***************************************************************************************************************************************************************************************/
void saveSettings() {
     DynamicJsonDocument jsonDoc(6144); // Adjusted size

    // WiFi settings
    jsonDoc["wifiSSID"] = wifiSSID;
    jsonDoc["wifiPassword"] = wifiPassword;
    Serial.println("*****************************************************************");
    Serial.println("WiFi settings:");
    Serial.println("SSID: " + wifiSSID);
    Serial.println("Password: " + wifiPassword);
    Serial.println("*****************************************************************");

    // Access Point (AP) settings
    jsonDoc["existingAPPassword"] = existingAPPassword;
    jsonDoc["newAPPassword"] = newAPPassword;
    jsonDoc["confirmAPPassword"] = confirmAPPassword;
    Serial.println("*****************************************************************");
    Serial.println("Access Point (AP) settings:");
    Serial.println("Existing Password: " + existingAPPassword);
    Serial.println("New Password: " + newAPPassword);
    Serial.println("Confirm Password: " + confirmAPPassword);
    Serial.println("*****************************************************************");

    // MQTT settings
    jsonDoc["mqttBroker"] = mqttBroker;
    jsonDoc["mqttUser"] = mqttUser;
    jsonDoc["mqttPassword"] = mqttPassword;
    jsonDoc["mqttPort"] = mqttPort;
    Serial.println("*****************************************************************");
    Serial.println("MQTT settings:");
    Serial.println("Broker: " + mqttBroker);
    Serial.println("User: " + mqttUser);
    Serial.println("Password: " + mqttPassword);
    Serial.println("Port: " + String(mqttPort));
    Serial.println("*****************************************************************");

    // Relay control devices
    JsonArray relayDevicesArray = jsonDoc.createNestedArray("relayDevice");
    for (size_t i = 0; i < 4; i++) {
        JsonObject device = relayDevicesArray.createNestedObject();
        device["identifier"] = relayDevice[i].identifier;
        device["name"] = relayDevice[i].name;
        device["manufacturer"] = relayDevice[i].manufacturer;
        device["model"] = relayDevice[i].model;
        device["location"] = relayDevice[i].location;
        Serial.println("*****************************************************************");
        Serial.println("Relay Device " + String(i) + " settings:");
        Serial.println("Identifier: " + relayDevice[i].identifier);
        Serial.println("Name: " + relayDevice[i].name);
        Serial.println("Manufacturer: " + relayDevice[i].manufacturer);
        Serial.println("Model: " + relayDevice[i].model);
        Serial.println("Location: " + relayDevice[i].location);
    }

    // Sensor settings
    JsonArray sensorsArray = jsonDoc.createNestedArray("sensor");
    for (size_t i = 0; i < 50; i++) {
        JsonObject sensor = sensorsArray.createNestedObject();
        sensor["identifier"] = Sensor[i].identifier;
        sensor["name"] = Sensor[i].name;
        sensor["manufacturer"] = Sensor[i].manufacturer;
        sensor["model"] = Sensor[i].model;
        sensor["location"] = Sensor[i].location;
        Serial.println("*****************************************************************");
        Serial.println("Sensor " + String(i) + " settings:");
        Serial.println("Identifier: " + Sensor[i].identifier);
        Serial.println("Name: " + Sensor[i].name);
        Serial.println("Manufacturer: " + Sensor[i].manufacturer);
        Serial.println("Model: " + Sensor[i].model);
        Serial.println("Location: " + Sensor[i].location);
    }

    // Other information
    jsonDoc["sw_version"] = sw_version;
    jsonDoc["hw_version"] = hw_version;
    jsonDoc["configuration"] = configuration;
    jsonDoc["chipID"] = chipID;
    Serial.println("*****************************************************************");
    Serial.println("Other information:");
    Serial.println("Software Version: " + sw_version);
    Serial.println("Hardware Version: " + hw_version);
    Serial.println("Configuration: " + configuration);
    Serial.println("Chip ID: " + String(chipID));
    Serial.println("*****************************************************************");

    // Network settings
    jsonDoc["ip"] = ip.toString();
    jsonDoc["gateway"] = gateway.toString();
    jsonDoc["subnet"] = subnet.toString();
    Serial.println("*****************************************************************");
    Serial.println("Network settings:");
    Serial.println("IP Address: " + ip.toString());
    Serial.println("Gateway: " + gateway.toString());
    Serial.println("Subnet Mask: " + subnet.toString());
    Serial.println("*****************************************************************");

    // Serialize JSON to string
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);

    // Write to SPIFFS
    writeFile(SPIFFS, CONFIG_FILE, jsonStr.c_str());

    Serial.println("Settings saved successfully");
}
/***************************************************************************************************************************************************************************************/
// Function to read a file from SPIFFS
String readFile(fs::FS &fs, const char *path) {
    File file = fs.open(path);
    String readfile;

    while (file.available()) {
        readfile = file.readString();
        file.flush();
    }
    file.close();
    return readfile;
}
/***************************************************************************************************************************************************************************************/

// Function to write a file to SPIFFS
void writeFile(fs::FS &fs, const char *path, const char *message) {
    File file = fs.open(path, FILE_WRITE);

    if (file && file.print(message))
        ;
    file.close();
}
/***************************************************************************************************************************************************************************************/

// Function to load settings from SPIFFS
void loadSettings() {
    String json = readFile(SPIFFS, CONFIG_FILE);

    if (json != "") {
        DynamicJsonDocument doc(6144); // Adjust size if necessary
        DeserializationError error = deserializeJson(doc, json);

        if (error) {
            Serial.println("Failed to parse config file");
            return;
        }

        JsonObject obj = doc.as<JsonObject>();

        // WiFi settings
        wifiSSID = obj["wifiSSID"] | "";
        wifiPassword = obj["wifiPassword"] | "";

        // Access Point (AP) settings
        existingAPPassword = obj["existingAPPassword"] | "";
        newAPPassword = obj["newAPPassword"] | "";
        confirmAPPassword = obj["confirmAPPassword"] | "";

        // MQTT settings
        mqttBroker = obj["mqttBroker"] | "";
        mqttUser = obj["mqttUser"] | "";
        mqttPassword = obj["mqttPassword"] | "";
        mqttPort = obj["mqttPort"] | 1883;

        // Relay control devices
        JsonArray relayDevicesArray = obj["relayDevice"].as<JsonArray>();
        for (int i = 0; i < 4; ++i) {
            if (relayDevicesArray.size() > i) {
                relayDevice[i].identifier = relayDevicesArray[i]["identifier"] | "default_ID";
                relayDevice[i].name = relayDevicesArray[i]["name"] | "default_Name";
                relayDevice[i].manufacturer = relayDevicesArray[i]["manufacturer"] | "default_Manufacturer";
                relayDevice[i].model = relayDevicesArray[i]["model"] | "default_Model";
                relayDevice[i].location = relayDevicesArray[i]["location"] | "default_Location";
            } else {
                // Handle case where less than 4 relay devices are present in config
                relayDevice[i].identifier = "default_ID";
                relayDevice[i].name = "default_Name";
                relayDevice[i].manufacturer = "default_Manufacturer";
                relayDevice[i].model = "default_Model";
                relayDevice[i].location = "default_Location";
            }
        }

        // Sensor settings
        JsonArray sensorsArray = obj["sensor"].as<JsonArray>();
        for (int i = 0; i < 50; ++i) {
            if (sensorsArray.size() > i) {
                Sensor[i].identifier = sensorsArray[i]["identifier"] | "default_ID";
                Sensor[i].name = sensorsArray[i]["name"] | "default_Name";
                Sensor[i].manufacturer = sensorsArray[i]["manufacturer"] | "default_Manufacturer";
                Sensor[i].model = sensorsArray[i]["model"] | "default_Model";
                Sensor[i].location = sensorsArray[i]["location"] | "default_Location";
            } else {
                // Handle case where less than 50 sensors are present in config
                Sensor[i].identifier = "default_ID";
                Sensor[i].name = "default_Name";
                Sensor[i].manufacturer = "default_Manufacturer";
                Sensor[i].model = "default_Model";
                Sensor[i].location = "default_Location";
            }
        }

        // Other information
        sw_version = obj["sw_version"] | "";
        hw_version = obj["hw_version"] | "";
        configuration = obj["configuration"] | "";
        chipID = obj["chipID"] | 0;

        // Network settings
        ip.fromString(obj["ip"] | "192.168.1.100");
        gateway.fromString(obj["gateway"] | "192.168.1.1");
        subnet.fromString(obj["subnet"] | "255.255.255.0");

        Serial.println("Settings loaded successfully");
        doc.clear();
    } else {
        Serial.println("No settings file found");
    }
}
/***************************************************************************************************************************************************************************************/
String generateSensorSettingsHTML() {
    String html = "<div id=\"sensor-container\">\n";
    
    // Iterate through each sensor and replace placeholders
    for (size_t i = 0; i < 50; ++i) {
        String placeholderPrefix = "%SENSOR";
        
        // Check if sensor data is not default before replacing
        if (Sensor[i].identifier != "default") {
            html.replace(placeholderPrefix + "ID%" + String(i + 1), Sensor[i].identifier);
        }
        if (Sensor[i].name != "default") {
            html.replace(placeholderPrefix + "NAME%" + String(i + 1), Sensor[i].name);
        }
        if (Sensor[i].manufacturer != "default") {
            html.replace(placeholderPrefix + "MANUFACTURER%" + String(i + 1), Sensor[i].manufacturer);
        }
        if (Sensor[i].model != "default") {
            html.replace(placeholderPrefix + "MODEL%" + String(i + 1), Sensor[i].model);
        }
        if (Sensor[i].location != "default") {
            html.replace(placeholderPrefix + "LOCATION%" + String(i + 1), Sensor[i].location);
        }
    }

    html += "</div>\n";

    return html;
}


void handleRoot() {
    String html = String(Config_html); // Assuming Config_html is your HTML template stored in html.h

    // Replace placeholders for WiFi settings
    html.replace("%SSID%", wifiSSID);
    html.replace("%PASSWORD%", wifiPassword);

    // Replace placeholders for Access Point (AP) settings
    html.replace("%APEXISTINGPASSWORD%", existingAPPassword);

    // Replace placeholders for MQTT settings
    html.replace("%BMQTTROKER%", mqttBroker);
    html.replace("%MQTTUSER%", mqttUser);
    html.replace("%MQTTPORT%", String(mqttPort));
    html.replace("%MQTTPASSWORD%", mqttPassword);

    // Replace placeholders for relay control devices
    for (int i = 0; i < 4; ++i) {
        String placeholderId = "%RLID" + String(i + 1) + "%";
        String placeholderLocation = "%RLLOCATION" + String(i + 1) + "%";
        String placeholderModel = "%RLMODEL" + String(i + 1) + "%";
        String placeholderManufacturer = "%RLMANUFACTURER" + String(i + 1) + "%";
        String placeholderName = "%RLNAME" + String(i + 1) + "%";

        html.replace(placeholderId, relayDevice[i].identifier);
        html.replace(placeholderLocation, relayDevice[i].location);
        html.replace(placeholderModel, relayDevice[i].model);
        html.replace(placeholderManufacturer, relayDevice[i].manufacturer);
        html.replace(placeholderName, relayDevice[i].name);
    }

    // Replace placeholders for sensor settings
    for (size_t i = 0; i < 50; ++i) {
        String placeholderPrefix = "%SENSOR";
        html.replace(placeholderPrefix + "ID%" + String(i + 1), Sensor[i].identifier);
        html.replace(placeholderPrefix + "NAME%" + String(i + 1), Sensor[i].name);
        html.replace(placeholderPrefix + "MANUFACTURER%" + String(i + 1), Sensor[i].manufacturer);
        html.replace(placeholderPrefix + "MODEL%" + String(i + 1), Sensor[i].model);
        html.replace(placeholderPrefix + "LOCATION%" + String(i + 1), Sensor[i].location);
    }

    // Replace placeholders for device info
    html.replace("%SWVERISONINFO%", sw_version);
    html.replace("%HWVERSIONINFO%", hw_version);
    html.replace("%CONFIGINFO%", configuration);

    // Send the HTML response
    server.send(200, "text/html", html); // Assuming 'server' is your WebServer object
}
/***************************************************************************************************************************************************************************************/
String prepareSensorData() {
    // Build sensor data as JSON or HTML content
    DynamicJsonDocument doc(6144); // Adjust the buffer size as per your data size

    // Default values for comparison
    String defaultIdentifier = "default_ID";
    String defaultName = "default_Name";
    String defaultManufacturer = "default_Manufacturer";
    String defaultModel = "default_Model";
    String defaultLocation = "default_Location";

    // Build sensor data as JSON
    JsonArray sensors = doc.createNestedArray("sensors");
    for (size_t i = 0; i < 50; ++i) {
        // Check if the sensor has default values
        if (strcmp(Sensor[i].identifier.c_str(), defaultIdentifier.c_str()) != 0 ||
            strcmp(Sensor[i].name.c_str(), defaultName.c_str()) != 0 ||
            strcmp(Sensor[i].manufacturer.c_str(), defaultManufacturer.c_str()) != 0 ||
            strcmp(Sensor[i].model.c_str(), defaultModel.c_str()) != 0 ||
            strcmp(Sensor[i].location.c_str(), defaultLocation.c_str()) != 0) {
            
            JsonObject sensor = sensors.createNestedObject();
            sensor["identifier"] = Sensor[i].identifier;
            sensor["name"] = Sensor[i].name;
            sensor["manufacturer"] = Sensor[i].manufacturer;
            sensor["model"] = Sensor[i].model;
            sensor["location"] = Sensor[i].location;
        }
    }

    // Serialize JSON to a string
    String sensorData;
    serializeJson(doc, sensorData);

    return sensorData;
}
/***************************************************************************************************************************************************************************************/
bool mountSPIFFS() {
  while (!SPIFFS.begin(true)) {
      delay(3000);
    }
}
/***************************************************************************************************************************************************************************************/
// Function to create and write default configuration to SPIFFS
void createConfigFile() {
    DynamicJsonDocument jsonDoc(6144);

    // Set default WiFi settings
    jsonDoc["wifiSSID"] = "YourWiFiSSID";
    jsonDoc["wifiPassword"] = "YourWiFiPassword";

    // Set default Access Point (AP) settings
    jsonDoc["existingAPPassword"] = "YourExistingAPPassword";
    jsonDoc["newAPPassword"] = "";
    jsonDoc["confirmAPPassword"] = "";

    // Set default MQTT settings
    jsonDoc["mqttBroker"] = "mqtt.example.com";
    jsonDoc["mqttUser"] = "mqttUser";
    jsonDoc["mqttPassword"] = "mqttPassword";
    jsonDoc["mqttPort"] = 1883;

    // Initialize relay control devices with default values for the first four entries
    JsonArray relayDevicesArray = jsonDoc.createNestedArray("relayDevices");
    for (int i = 0; i < 4; ++i) {
        JsonObject relayDeviceObj = relayDevicesArray.createNestedObject();
        relayDeviceObj["identifier"] = "default_ID";
        relayDeviceObj["name"] = "default_Name";
        relayDeviceObj["manufacturer"] = "default_Manufacturer";
        relayDeviceObj["model"] = "default_Model";
        relayDeviceObj["location"] = "default_Location";
    }

    // Initialize sensor settings with default values for the first 50 entries
    JsonArray sensorsArray = jsonDoc.createNestedArray("sensors");
    for (int i = 0; i < 50; ++i) {
        JsonObject sensorObj = sensorsArray.createNestedObject();
        sensorObj["identifier"] = "default_ID";
        sensorObj["name"] = "default_Name";
        sensorObj["manufacturer"] = "default_Manufacturer";
        sensorObj["model"] = "default_Model";
        sensorObj["location"] = "default_Location";
    }

    // Set default other information
    jsonDoc["sw_version"] = "1.0";
    jsonDoc["hw_version"] = "1.0";
    jsonDoc["configuration"] = "Default Configuration";
    jsonDoc["chipID"] = "xxxxxxxx";

    // Set default network settings
    jsonDoc["ip"] = "192.168.1.100";
    jsonDoc["gateway"] = "192.168.1.1";
    jsonDoc["subnet"] = "255.255.255.0";

    // Serialize JSON to string
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);

    // Write to SPIFFS
    writeFile(SPIFFS, CONFIG_FILE, jsonStr.c_str());
}
