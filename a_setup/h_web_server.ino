// helper function that creates the web server and launch it
void launchWeb()
{
  Serial.println ("starting web site");
  createWebServer();
  server.begin();
  
}

// make a web server that sends a html file
// and handles submission from client
void createWebServer()
{
  server.on("/", handleMain);
  server.on("/setting", handleSubmitValue);
  server.on("/style.css", sendWifiCssFile);
}

void handleMain()
{
  //File html_file = SPIFFS.open("/index.html", "r"); // working html submission template
  File html_file = SPIFFS.open("/main.html", "r");
  server.streamFile(html_file, "text/html");
  html_file.close();
  
}

void sendWifiCssFile()
{
  File css_file = SPIFFS.open("/style.css", "r");
  server.streamFile(css_file, "text/css");
  css_file.close();
}


// accept values from client and write it to eeprom
void handleSubmitValue()
{
  Serial.println ("handling submitted value");
  int nextAddress = 0;
  String wifiName = server.arg("ssid");
  String wifiPass = server.arg("pass");
  String portNum = server.arg ("port");
  String sleep_timer = server.arg("sleep_timer");
  String sensor_present = server.arg("installed_sensor");
  server.send(200, "text/plain", "credential recorded!!");

  // only store credentials if wifi name and pass exists
  // and port number is a valid 5 digit positive number
  // the sleep interval is a valid 5 digit positive number
  if (testWifi(wifiName, wifiPass) && portNum.toInt() > 0 && sleep_timer.toInt() > 0)
  {
    /*
     * index 0: wifi name
     * index 1: wifi password
     * index 2: location name
     * index 3: port number
     * index 4: server IP
     * index 5: sleep timer
     * index 6: light sensor insatlled?
     */    
    nextAddress = writeToRom (wifiName, nextAddress);
    nextAddress = writeToRom (wifiPass, nextAddress);
    nextAddress = writeToRom (server.arg ("location_name"), nextAddress);
    nextAddress = writeToRom (portNum, nextAddress);
    nextAddress = writeToRom (server.arg ("serverIP"), nextAddress);
    nextAddress = writeToRom (sleep_timer, nextAddress);
    nextAddress = writeToRom (sensor_present, nextAddress);
    
    // write an S to the last byte in the rom for verification
    // This letter is needed when the esp first start up
    // and about to read the rom, it needs to know if there are anything
    // writen to the rom before
    // if this doesn't exist it'll not read the rom
    writeToRom ("S", 510);
    
    EEPROM.commit(); // write value from memory to the physical flash
    Serial.println ("restarting esp");
    ESP.reset(); // restart the esp to close all server and access point
  }
  else
  {
    server.send(200, "text/plain", "incorrect values!");
  }
}
