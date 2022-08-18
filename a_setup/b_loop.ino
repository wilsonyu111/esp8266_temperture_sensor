void loop()
{
  String temp = getTemp();
  String hudm = getHudm();
  String light_status = "N/A";

  if (temp == "")
  {
    Serial.println("Failed to read temp from DHT sensor!");
  }
  else
  {
    Serial.println((String)"temp: " + temp);
  }
  
  if (hudm == "")
  {
    Serial.println ("Failed to read hud from DHT sensor!");
  }
  else
  {
    Serial.println((String)"hud: " + hudm);
  }

  if (installed_light)
  {
    light_status = getLightStat();
  }
  
  sendPost (hudm, temp, light_status);
  
  //Serial.println("wait 30 secs");
  //ESP.deepSleep(30e6); // go to deepsleep for 30 seconds and then wake up
  drd.loop();
  delay (sleep_interval);
}
