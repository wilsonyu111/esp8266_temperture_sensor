#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <DoubleResetDetector.h>
#include <EEPROM.h>
#define DHTPIN 5     // Digital pin connected to the DHT sensor, D1 pin on esp 8266
#define DHTTYPE    DHT11     // DHT 11
#define DRD_TIMEOUT 10 // double reset timeout
#define DRD_ADDRESS 0 // double reset address
 
const int arrayLength = 7;
const int LIGHT_INPUT_PIN = 4; // D2 PIN
String infoArr [arrayLength]; // for storing wifi credential read from eeprom
const String tempType = "F";
String SERVER_PATH; // "http://" + server_ipadd + ":" + port+ "/sendtoserver";
WiFiClient client; // for invoking wifi connection
DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
ESP8266WebServer server(80); // making a server object that listens on port 80
DHT dht(DHTPIN, DHTTYPE);
long sleep_interval; // how long before the sensor will be read again
const long baud_rate = 115200;
const int eeprom_size = 512; // bytes of eeprom initializing, esp only have 512 bytes
String ssid;
String password;
String LOCATION;
String port;
String server_ipadd;
bool installed_light;
bool resetWifi = false;
bool APactive = false; // if the AP is active make sure wifi cannot be connected


void debugs()
{
  Serial.println ("printing credentials");
  for (int i=0; i<arrayLength; i++)
  {
    Serial.println (infoArr[i]);
  }
}

void setup() {

  EEPROM.begin(eeprom_size); //Initialasing EEPROM
  pinMode(LED_BUILTIN, OUTPUT); // set the esp 8266 LED  to output mode
  Serial.begin(baud_rate); // initialize baud rate
  dht.begin(); // sensor begin reading

  // first check to see if the eeprom has been written yet
  // if the last byte is an S (stored)
  // read credentials from 

  String storeChar = "";
  /*
  storeChar += char(EEPROM.read(eeprom_size-200));
  */
  Serial.print (EEPROM.read(510));

  checkDoubleReset (); // check if rest button is pressed twice

  // if the last character in the eeprom is not an S
  // or reset wifi is set to true
  // this will reset all wifi credentials
  // and start the SPIFFS file system
  // for serving HTML and CSS files
  if (EEPROM.read(510) != 83 || resetWifi)
  {
    Serial.println("clearing rom");
    // clear the rom first before writing
    clearRom (eeprom_size);
    if (SPIFFS.begin()) // start SPIFFS for reading HTML and CSS file
    {
      Serial.println("opened SPIFFS");
    }
    else
    {
      Serial.println("failed to open SPIFFS");
    }
    
    // start the web server and AP to get the credentials
    setupAP();
    launchWeb();

    Serial.println("waiting for credentials");
    Serial.println ("");
    while ((WiFi.status() != WL_CONNECTED) && APactive)
    {

      Serial.print(".");
      delay(200);
      server.handleClient();
    }
    
  }
  // credential already exist
  // read the info from EEPROM
  else
  {
    //helper function that reads everything in the rom and store it infoArr
    /*
     * index 0: wifi name
     * index 1: wifi password
     * index 2: location name
     * index 3: port number
     * index 4: server IP
     * index 5: sleep timer
     * index 6: light sensor insatlled?
     */   
    readFromRomHelper (); // read credential from EEPROM
    ssid = infoArr[0]; // wifi name
    password = infoArr[1]; // wifi password
    LOCATION = infoArr[2]; // room location
    port = infoArr[3]; // listening port of the server
    server_ipadd = infoArr[4]; // server ip address
    sleep_interval = infoArr[5].toInt() * 60000;// sleep interval in milliseconds
    installed_light = infoArr[6].toInt(); // is the esp equiped with light sensor? The value is either 1 or 0, true or false
    SERVER_PATH = "http://" + server_ipadd + ":" + port + "/sendtoserver"; // full server http address for raw data to be sent to
    //debugs(); // print out all credentials
    wifiConnect(); // connect to wifi
  }
}
