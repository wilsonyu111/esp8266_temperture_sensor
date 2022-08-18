void wifiConnect()
{
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) // loop that wait for wifi to connect before moving to reading sensors
  {
    
    digitalWrite(LED_BUILTIN, LOW); // turn on led 
    delay(800);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH); // turn off led
    delay(800);
  }
}

bool testWifi(String wifiName, String wifiPass)
{
  Serial.println("testing wifi");
  int attempts = 120;
  WiFi.begin(wifiName, wifiPass);

  // try to connect to the wifi for 20 times
  // if it fails return false and don't store the credential
  while (WiFi.status() != WL_CONNECTED && attempts > 0) // loop that wait for wifi to connect before moving to reading sensors
  {    
    digitalWrite(LED_BUILTIN, LOW); // turn on led 
    delay(200);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH); // turn off led
    delay(200);
    attempts --;
  }

  if (attempts == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}
