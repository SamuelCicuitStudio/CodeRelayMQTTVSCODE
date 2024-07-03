const char* Config_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuration Page</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f0f0;
            
        }
        .container {
            width: 700px;
            height: 630px;
            background-color: #fff;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            display: flex;
            border-radius: 30px; /* Rounded corners */
        }
        .tabs {
            width: 200px;
            border-right: 1px solid #ddd;
            display: flex;
            flex-direction: column;
            
        }
        .tabs button {
            display: block;
            width: 100%;
            padding: 10px;
            background: #f1f1f1;
            border: none;
            border-bottom: 1px solid #ddd;
            cursor: pointer;
            text-align: left;
        }
        .tabs button:hover {
            background: #ddd;
        }
        .tabs button.active {
            background: #ccc;
        }
        .tab-content {
            flex-grow: 1;
            padding: 20px;
            display: none;
        }
        .tab-content.active {
            display: block;
        }
        .form-group {
            margin-bottom: 15px !important;
        }
        .form-group label {
            display: block !important;
            margin-bottom: 2px !important;
        }
        .form-group input[type="text"],
        .form-group input[type="password"] {
            width: calc(100% - 50px) !important; /* Adjusted width */
            padding: 5px !important;
            box-sizing: border-box !important;
            border: 1px solid #ccc !important;
            border-radius: 4px !important;
            font-size: 14px !important;
            transition: border-color 0.2s ease-in-out !important;
        }
        .form-group input[type="text"]:focus,
        .form-group input[type="password"]:focus {
            outline: none !important;
            border-color: #007bff !important;
        }
        .next-button,
        .back-button,
        .add-button {
            padding: 8px 12px;
            background-color: #007bff;
            color: white;
            border: none;
            cursor: pointer;
            margin-top: 20px;
        }
        .next-button:hover,
        .back-button:hover,
        .add-button:hover {
            background-color: #0056b3;
        }
        
        .delete-button {
            padding: 8px 12px;
            background-color: #e62e0d;
            color: white;
            border: none;
            cursor: pointer;
            margin-top: 20px;
        }
        
        .delete-button:hover {
            background-color: #8d1b07;
        }
        
        .save-button {
    padding: 10px 20px !important;
    background-color: #4CAF50 !important; /* Green */
    color: white !important;
    border: none !important;
    cursor: pointer !important;
    margin-top: auto !important; /* Pushes the button to the bottom */
    align-self: flex-start !important; /* Aligns the button to the left */
}
.save-button:hover {
    background-color: #45a049 !important; /* Darker Green on hover */
}

.reboot-button {
    padding: 10px 20px !important;
    background-color: #3498db !important; /* Blue */
    color: white !important;
    border: none !important;
    cursor: pointer !important;
    margin-top: auto !important; /* Pushes the button to the bottom */
    align-self: flex-start !important; /* Aligns the button to the left */
}
.reboot-button:hover {
    background-color: #2980b9 !important; /* Darker Blue on hover */
}

.refresh-button {
    padding: 10px 20px !important;
    background-color: #f39c12 !important; /* Orange */
    color: white !important;
    border: none !important;
    cursor: pointer !important;
    margin-top: auto !important; /* Pushes the button to the bottom */
    align-self: flex-start !important; /* Aligns the button to the left */
}
.refresh-button:hover {
    background-color: #e67e22 !important; /* Darker Orange on hover */
}

        
        .sensor-index-box {
            display: flex;
            align-items: center;
            background-color: #e74c3c;
            padding: 5px 10px;
            border-radius: 4px;
            color: #fff;
        }
        #sensor-index {
            font-size: 1.2em;
            font-weight: bold;
            color: #e74c3c;
            margin-left: 10px;
        }
        #sensor-index-label {
            font-size: 1.2em;
            color: #2c3e50;
        }

        .form-group {
            margin-bottom: 15px;
        }
        .form-group label {
            display: block;
            margin-bottom: 5px;
        }
        .form-group input[type="text"],
        .form-group input[type="password"] {
            width: 100%;
            padding: 8px;
            box-sizing: border-box;
            border: 1px solid #ccc;
            border-radius: 4px;
            font-size: 14px;
            transition: border-color 0.2s ease-in-out;
        }
        .form-group input[type="text"]:focus,
        .form-group input[type="password"]:focus {
            outline: none;
            border-color: #007bff;
        }
        h3 {
        font-size: 24px;
        color: #007bff;
        border-bottom: 2px solid #007bff;
        padding-bottom: 5px;
        margin-bottom: 15px;
    }
    </style>
</head>
<body>
</head>
<body>

<div class="container">
    <div class="tabs">
        <button class="tablink" onclick="openTab(event, 'WifiSettings')" id="defaultOpen">WiFi Settings</button>
        <button class="tablink" onclick="openTab(event, 'ApSettings')">Hot-Spot Settings</button>
        <button class="tablink" onclick="openTab(event, 'MQTTSettings')">MQTT Settings</button>
        <button class="tablink" onclick="openTab(event, 'RelayControl')">Relay Control</button>
        <button class="tablink" onclick="openTab(event, 'SensorSettings')">Sensor Settings</button>
        <button class="tablink" onclick="openTab(event, 'DeviceInfo')">Device Info</button>
        <button class="save-button" onclick="saveSettings()">Save Settings</button>
        <div class="action-buttons">
            <button class="refresh-button" onclick="refreshPage()">Refresh</button>
            <button class="reboot-button" onclick="rebootDevice()">Reboot</button>
        </div>

    </div>
    
    <div class="tab-content" id="WifiSettings">
        <h3>WiFi Settings</h3>
        <div class="form-group">
            <label for="ssid">SSID</label>
            <input type="text" id="ssid" name="ssid" placeholder="%SSID%">
        </div>
        <div class="form-group">
            <label for="password">Password</label>
            <input type="password" id="password" name="password" placeholder="%PASSWORD%">
        </div>
    </div>

    <div class="tab-content" id="ApSettings">
        <h3>HotSpot Settings</h3>
        <div class="form-group">
            <label for="ap-existing-password">Existing Password</label>
            <input type="password" id="ap-existing-password" name="ap-existing-password" >
        </div>
        <div id="newPasswordSection" style="display:none;">
            <div class="form-group">
                <label for="ap-new-password">New Password</label>
                <input type="password" id="ap-new-password" name="ap-new-password">
            </div>
            <div class="form-group">
                <label for="ap-confirm-password">Confirm New Password</label>
                <input type="password" id="ap-confirm-password" name="ap-confirm-password">
            </div>
        </div>
        <button id="showNewPasswordBtn" onclick="showNewPasswordSection()">Enter New Password</button>
    </div>
    
    

    <div class="tab-content" id="MQTTSettings">
        <h3>MQTT Settings</h3>
        <div class="form-group">
            <label for="broker">Broker</label>
            <input type="text" id="broker" name="broker" placeholder="%BMQTTROKER%">
        </div>
        <div class="form-group">
            <label for="user">User</label>
            <input type="text" id="user" name="user" placeholder="%MQTTUSER%">
        </div>
        <div class="form-group">
            <label for="port">Port</label>
            <input type="text" id="port" name="port" placeholder="%MQTTPORT%">
        </div>
        <div class="form-group">
            <label for="mqttpassword">Password</label>
            <input type="password" id="mqttpassword" name="mqttpassword" placeholder="%MQTTPASSWORD%">
        </div>
    </div>

    <div class="tab-content" id="RelayControl">
        <h3>Relay Control</h3>
        <div id="relay-container">
            <div id="device-1" class="device-settings">
                <h4>Device 1</h4>
                <div class="form-group">
                    <label for="device1-identifier">Identifier</label>
                    <input type="text" id="device1-identifier" name="device1-identifier" placeholder="%RLID1%">
                </div>
                <div class="form-group">
                    <label for="device1-name">Name</label>
                    <input type="text" id="device1-name" name="device1-name" placeholder="%RLNAME1%">
                </div>
                <div class="form-group">
                    <label for="device1-manufacturer">Manufacturer</label>
                    <input type="text" id="device1-manufacturer" name="device1-manufacturer" placeholder="%RLMANUFACTURER1%">
                </div>
                <div class="form-group">
                    <label for="device1-model">Model</label>
                    <input type="text" id="device1-model" name="device1-model" placeholder="%RLMODEL1%">
                </div>
                <div class="form-group">
                    <label for="device1-location">Location</label>
                    <input type="text" id="device1-location" name="device1-location" placeholder="%RLLOCATION1%">
                </div>
            </div>
            <div id="device-2" class="device-settings" style="display:none">
                <h4>Device 2</h4>
                <div class="form-group">
                    <label for="device2-identifier">Identifier</label>
                    <input type="text" id="device2-identifier" name="device2-identifier" placeholder="%RLID2%">
                </div>
                <div class="form-group">
                    <label for="device2-name">Name</label>
                    <input type="text" id="device2-name" name="device2-name"  placeholder="%RLNAME2%">
                </div>
                <div class="form-group">
                    <label for="device2-manufacturer">Manufacturer</label>
                    <input type="text" id="device2-manufacturer" name="device2-manufacturer" placeholder="%RLMANUFACTURER2%">
                </div>
                <div class="form-group">
                    <label for="device2-model">Model</label>
                    <input type="text" id="device2-model" name="device2-model" placeholder="%RLMODEL2%">
                </div>
                <div class="form-group">
                    <label for="device2-location">Location</label>
                    <input type="text" id="device2-location" name="device2-location" placeholder="%RLLOCATION2%">
                </div>
            </div>
            <div id="device-3" class="device-settings" style="display:none">
                <h4>Device 3</h4>
                <div class="form-group">
                    <label for="device3-identifier">Identifier</label>
                    <input type="text" id="device3-identifier" name="device3-identifier" placeholder="%RLID3%">
                </div>
                <div class="form-group">
                    <label for="device3-name">Name</label>
                    <input type="text" id="device3-name" name="device3-name" placeholder="%RLNAME3%">
                </div>
                <div class="form-group">
                    <label for="device3-manufacturer">Manufacturer</label>
                    <input type="text" id="device3-manufacturer" name="device3-manufacturer" placeholder="%RLMANUFACTURER3%"> 
                </div>
                <div class="form-group">
                    <label for="device3-model">Model</label>
                    <input type="text" id="device3-model" name="device3-model" placeholder="%RLMODEL3%">
                </div>
                <div class="form-group">
                    <label for="device3-location">Location</label>
                    <input type="text" id="device3-location" name="device3-location" placeholder="%RLLOCATION3%">
                </div>
            </div>
            <div id="device-4" class="device-settings" style="display:none">
                <h4>Device 4</h4>
                <div class="form-group">
                    <label for="device4-identifier">Identifier</label>
                    <input type="text" id="device4-identifier" name="device4-identifier" placeholder="%RLID4%" >
                </div>
                <div class="form-group">
                    <label for="device4-name">Name</label>
                    <input type="text" id="device4-name" name="device4-name" placeholder="%RLNAME4%">
                </div>
                <div class="form-group">
                    <label for="device4-manufacturer">Manufacturer</label>
                    <input type="text" id="device4-manufacturer" name="device4-manufacturer" placeholder="%RLMANUFACTURER4%">
                </div>
                <div class="form-group">
                    <label for="device4-model">Model</label>
                    <input type="text" id="device4-model" name="device4-model" placeholder="%RLMODEL4%">
                </div>
                <div class="form-group">
                    <label for="device4-location">Location</label>
                    <input type="text" id="device4-location" name="device4-location" placeholder="%RLLOCATION4%">
                </div>
            </div>
        </div>
        <button class="back-button" onclick="prevDevice()">Back</button>
        <button class="next-button" onclick="nextDevice()">Next</button>
    </div>

    <div class="tab-content" id="SensorSettings">
        <h3>Sensor Settings</h3>
        <button class="add-button" onclick="addSensor()">Add Sensor</button>
        <div id="sensor-container">
            <!-- Sensors will be dynamically added here -->
        </div>
        <div class="nav-buttons">
            <span>Number of Sensors: </span><span id="sensor-index">0</span>
            <button class="back-button" onclick="prevSensor()">Back</button>
            <button class="next-button" onclick="nextSensor()">Next</button>
            <button class="delete-button" onclick="deleteSensor()">Delete Sensor</button>
        </div>
    </div>
    
    
    <div class="tab-content" id="DeviceInfo">
        <h3>Device Info</h3>
        <div class="form-group">
            <label for="sw-version">SW Version</label>
            <input type="text" id="sw-version" name="sw-version" placeholder="%SWVERISONINFO%">
        </div>
        <div class="form-group">
            <label for="hw-version">HW Version</label>
            <input type="text" id="hw-version" name="hw-version" placeholder="%HWVERSIONINFO%">
        </div>
        <div class="form-group">
            <label for="configuration">Configuration</label>
            <input type="text" id="configuration" name="configuration" placeholder="%CONFIGINFO%">
        </div>
    </div></div>

<script>

let currentSensor = 0;
let currentDevice = 0;


document.getElementById("defaultOpen").click();

let isSetDataExecuted = false;

function openTab(evt, tabName) {
    let i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tab-content");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablink");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";

    if (tabName === "SensorSettings" && !isSetDataExecuted) {
        setData();
        isSetDataExecuted = true; // Ensure setData executes only once
    }
}


// Device Controls
function nextDevice() {
    let devices = document.getElementsByClassName("device-settings");
    if (currentDevice < devices.length - 1) {
        devices[currentDevice].style.display = "none";
        currentDevice++;
        devices[currentDevice].style.display = "block";
        updateDeviceIndex();
    }
}

function prevDevice() {
    let devices = document.getElementsByClassName("device-settings");
    if (currentDevice > 0) {
        devices[currentDevice].style.display = "none";
        currentDevice--;
        devices[currentDevice].style.display = "block";
        updateDeviceIndex();
    }
}
// Sensor Controls

function nextSensor() {
    let sensors = document.getElementsByClassName("sensor-settings");
    if (currentSensor < sensors.length - 1) {
        sensors[currentSensor].style.display = "none";
        currentSensor++;
        sensors[currentSensor].style.display = "block";
        updateSensorIndex();
    }
}

function prevSensor() {
    let sensors = document.getElementsByClassName("sensor-settings");
    if (currentSensor > 0) {
        sensors[currentSensor].style.display = "none";
        currentSensor--;
        sensors[currentSensor].style.display = "block";
        updateSensorIndex();
    }
}

function setData() {
    // Fetch sensor data from the server
    fetch('/api/getSensorData', {
        method: 'POST', // Ensure this matches the server's handling
        headers: {
            'Content-Type': 'application/json'
        }
        // No body needed for a POST request in this case
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Failed to fetch sensor data');
        }
        return response.json();
    })
    .then(data => {
        // Buffer received data
        let sensorData = data.sensors; // Assuming data is in { sensors: [...] } format

        // Use addSensor function to populate sensor fields
        sensorData.forEach((sensor, index) => {
            addSensor(); // Add a new sensor field

            // Populate the added sensor field with data
            document.getElementById(`sensor-${index + 1}-identifier`).value = sensor.identifier;
            document.getElementById(`sensor-${index + 1}-name`).value = sensor.name;
            document.getElementById(`sensor-${index + 1}-manufacturer`).value = sensor.manufacturer;
            document.getElementById(`sensor-${index + 1}-model`).value = sensor.model;
            document.getElementById(`sensor-${index + 1}-location`).value = sensor.location;
        });
    })
    .catch(error => {
        console.error('Error fetching or processing sensor data:', error);
        alert('Failed to fetch or process sensor data. Please try again.');
    });
}

function addSensor() {
    let container = document.getElementById("sensor-container");
    let sensorIndex = container.children.length + 1;
    let sensorId = `sensor-${sensorIndex}`;
    let sensorHtml = `
        <div class="sensor-settings" style="display: ${sensorIndex === 1 ? 'block' : 'none'}">
            <h4>Sensor ${sensorIndex}</h4>
            <div class="form-group">
                <label for="${sensorId}-identifier">Identifier</label>
                <input type="text" id="${sensorId}-identifier" name="${sensorId}-identifier" placeholder="%SENSORID${sensorIndex}%">
            </div>
            <div class="form-group">
                <label for="${sensorId}-name">Name</label>
                <input type="text" id="${sensorId}-name" name="${sensorId}-name" placeholder="%SENSORNAME${sensorIndex}%">
            </div>
            <div class="form-group">
                <label for="${sensorId}-manufacturer">Manufacturer</label>
                <input type="text" id="${sensorId}-manufacturer" name="${sensorId}-manufacturer" placeholder="%SENSORMANUFACTURER${sensorIndex}%">
            </div>
            <div class="form-group">
                <label for="${sensorId}-model">Model</label>
                <input type="text" id="${sensorId}-model" name="${sensorId}-model" placeholder="%SENSORMODEL${sensorIndex}%">
            </div>
            <div class="form-group">
                <label for="${sensorId}-location">Location</label>
                <input type="text" id="${sensorId}-location" name="${sensorId}-location" placeholder="%SENSORLOCATION${sensorIndex}%">
            </div>
        </div>
    `;
    container.insertAdjacentHTML('beforeend', sensorHtml);
    updateSensorIndex();
}


    function showNewPasswordSection() {
    var existingPassword = document.getElementById("ap-existing-password").value;

    // AJAX request to validate existing password
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/validateExistingPassword", true);
    xhr.setRequestHeader("Content-Type", "application/json");

    xhr.onload = function () {
        if (xhr.status === 200) {
            var response = JSON.parse(xhr.responseText);
            if (response.valid === true) {
                document.getElementById("newPasswordSection").style.display = "block";
                document.getElementById("showNewPasswordBtn").style.display = "none";
            } else {
                alert("Existing password incorrect. Please try again.");
            }
        } else {
            alert("Error validating existing password. Please try again later.");
        }
    };

    var data = JSON.stringify({ password: existingPassword });
    xhr.send(data);
}

    function updateSensorIndex() {
        document.getElementById("sensor-index").textContent = document.getElementById("sensor-container").children.length;
    }

 function saveSettings() {
    function getValueOrPlaceholder(id) {
        const element = document.getElementById(id);
        const value = element.value.trim();
        return value !== '' ? value : element.placeholder;
    }

    // Gather all settings data from the form
    let settingsData = {
        wifi: {
            ssid: getValueOrPlaceholder("ssid"),
            password: getValueOrPlaceholder("password")
        },
        apSettings: {
            existingPassword: getValueOrPlaceholder("ap-existing-password"),
            newPassword: getValueOrPlaceholder("ap-new-password"),
            confirmNewPassword: getValueOrPlaceholder("ap-confirm-password")
        },
        mqtt: {
            broker: getValueOrPlaceholder("broker"),
            user: getValueOrPlaceholder("user"),
            port: getValueOrPlaceholder("port"),
            mqttpassword: getValueOrPlaceholder("mqttpassword") // Handle MQTT password field
        },
        relayControl: {
            devices: []
        },
        sensors: []
    };

    // Gather relay control devices data
    let deviceContainers = document.getElementsByClassName("device-settings");
    for (let i = 0; i < deviceContainers.length; i++) {
        let deviceData = {
            identifier: getValueOrPlaceholder(`device${i + 1}-identifier`),
            name: getValueOrPlaceholder(`device${i + 1}-name`),
            manufacturer: getValueOrPlaceholder(`device${i + 1}-manufacturer`),
            model: getValueOrPlaceholder(`device${i + 1}-model`),
            location: getValueOrPlaceholder(`device${i + 1}-location`)
        };
        settingsData.relayControl.devices.push(deviceData);
    }

    // Gather sensor settings data
    let sensorContainers = document.getElementsByClassName("sensor-settings");
    for (let i = 0; i < sensorContainers.length; i++) {
        let sensorData = {
            identifier: getValueOrPlaceholder(`sensor-${i + 1}-identifier`),
            name: getValueOrPlaceholder(`sensor-${i + 1}-name`),
            manufacturer: getValueOrPlaceholder(`sensor-${i + 1}-manufacturer`),
            model: getValueOrPlaceholder(`sensor-${i + 1}-model`),
            location: getValueOrPlaceholder(`sensor-${i + 1}-location`)
        };
        settingsData.sensors.push(sensorData);
    }

    // Send settingsData to ESP32 via AJAX
    fetch('/api/saveSettings', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(settingsData),
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log('Settings saved successfully:', data);
        alert('Settings saved successfully!');
    })
    .catch(error => {
        console.error('Error saving settings:', error);
        alert('Error saving settings. Please try again.');
    });
}



    function rebootDevice() {
    alert('Rebooting the Device!');
    
    // Send an AJAX POST request to the ESP32 to trigger reboot
    fetch('/api/reboot', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ reboot: true }),
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log('Reboot request sent successfully:', data);
    })
    .catch(error => {
        console.error('Error sending reboot request:', error);
    });
}

    function refreshPage() {
    location.reload(); // Reloads the current page
}
function deleteSensor() {
    let sensors = document.getElementsByClassName("sensor-settings");
    if (currentSensor >= 0 && currentSensor < sensors.length) {
        sensors[currentSensor].remove();
        currentSensor = Math.min(currentSensor, sensors.length - 1); // Adjust currentSensor index
        updateSensorIndex();
        prevSensor();
    }
}
</script>
</body>
</html>)rawliteral";
