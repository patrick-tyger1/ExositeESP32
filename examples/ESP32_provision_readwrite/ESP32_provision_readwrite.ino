//*****************************************************************************
//
// ESP32 Provision and Read from Exosite   - An example of how to use the ExositeESP32 
//                                          library with a ESP32 Dev Board for basic
//                                          provisioning, reading, and writing. 
//                                          The ExositeESP32 library was ported over 
//                                          from the standard Exosite library v2.5.3 and
//                                          works identically.
//
// Requires the ESP32 Arduino Core installed
//
//*****************************************************************************

// Libraries
#include <WiFi.h>
#include <ExositeESP32.h>

WiFiClient client;
Exosite exosite(&client);

byte macData[6];
char macString[18];
char testChar[41] = {0};
const char* ssid = "<WIFI SSID Here>";
const char* pass = "<WIFI Password Here>";


void setup() {
  initializeHardware();
  delay(3000);
  newConnection(); // readWrite


//  exosite.write(read);
//  exosite.read();
//  exosite.listAvailableContent();
//  exosite.downloadContent();
}
void loop() {
  int ts = millis();
  String readString = "uptime";
  String writeString = "uptime=";
  String returnString = "";

  writeString.concat(ts);
  Serial.print("Write string: "); Serial.println(writeString);
  exosite.write(writeString);
  delay(1000);
  exosite.read(readString, returnString);
  Serial.print("Read return: "); Serial.println(returnString);
  delay(1000);
  
  delay(30000);
}


/*==============================================================================
* initializeHardware 
*
* Configure externally driven functionality
*=============================================================================*/
void initializeHardware() {
  Serial.begin(115200);
  Serial.println("Booting ESP32...");
  exosite.begin();
  connectWifi();
  delay(3000);
  ESPprovision();
}

/*==============================================================================
* connectWifi 
*
* Use in setup to connect to local Wifi network
*=============================================================================*/
void connectWifi(){
//  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);

  byte ledStatus = LOW;
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  delay(100);
  
  // Create MAC Address String in the format FF:FF:FF:FF:FF:FF
  WiFi.macAddress(macData);
  snprintf(macString, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
           macData[5], macData[4], macData[3], macData[2], macData[1], macData[0]);
  // Print Some Useful Info
  Serial.print(F("MAC Address: "));
  Serial.println(macString);
}

/*==============================================================================
* ESPprovision
*
* Activate the ESP32 to specified vendor and model and get a CIK
*=============================================================================*/
void ESPprovision(){
  String returnString = "";
  while(client.connected()==0){
    exosite.provision("exosite", "arduinoyun-generic", macString);
  }
}

/*==============================================================================
* newConnection
*
* Read inbound dataports and redfine internal variables
*=============================================================================*/
int newConnection() {
  String writeString = "";
  String readString = "msg";
  String returnString = "";
  
  if(exosite.writeRead(writeString, readString, returnString)) {
    Serial.println("Data Retrieve Successful");
    Serial.println(returnString);

  }
  else {
    Serial.println("Error: Connection to portal unsuccessful. Check Connection");  
  }
  return true;
}
