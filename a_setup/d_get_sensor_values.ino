String getHudm ()// get hudmidity 
{
  float newH = dht.readHumidity();
  // if humidity read failed, don't change h value 
  if (isnan(newH))// if 
  {
    return "";
  }

  return (String) newH;
}

String getTemp ()
{
  float newT;
  
  if (tempType == "C")// if temperture type is C get temp in celsius
  {
    newT = dht.readTemperature();
  }
  else // if temperture type is C get temperature as Farenheit
  {
    newT = dht.readTemperature(true);
  }
  
  if (isnan(newT))
  {
    return "";
  }
  
  return (String)newT;
}

/*
 * The get light function returns "off" state when the pin is high
 * otherwise it'll return "on" state when the pin is low
 * this can be changed by making the pin pulling low
 * during the startup state
 * The input pin is D2
 */
String getLightStat()
{
  int light_sensor_state = digitalRead(LIGHT_INPUT_PIN);
  if (light_sensor_state)
  {
    return "off";
  }
  else
  {
    return "on";
  }
}
